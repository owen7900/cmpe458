/*
 *      Computer Systems Research Group
 *      University of Toronto
 *
 *      File:   ptc.c  v5.1
 *      Author: Alan Rosselet
 *              VAX and SUN conversions by J.R. Cordy, Queen's University
 *
 *      Date:   28 Nov 1980 
 *              (Revised v5.1 - 8 Feb 2021)
 *
 *      Copyright (C) 1980, the University of Toronto
 */

/* This is the "ptc" command which executes the PT compiler. */

#include <unistd.h>
#include <stdio.h>
#include <string.h>

char *usage = 
    "ptc [-help] [-O] [-i] [-I] [-S] [-c] [-L lib] [-T] [-oN] [-tN] [-nDD] progname.pt";

/* The PT Compiler Library (changeable via the -L option) */
char *library =         "/usr/local/lib/pt";

/* The PT Compiler Passes (in the library) */
char stdIds[256], scanSst[256], parser[256], parseSst[256], semantic[256],
     semanSst[256], coder[256], coderSst[256], ptam[256], ptamSst[256], monitor[256];
#define STDIDS          "/stdIdentifiers"
#define SCANSST         "/scan.sst"
#define PARSER          "/parser.out"
#define PARSESST        "/parser.sst"
#define SEMANTIC        "/semantic.out"
#define SEMANSST        "/semantic.sst"
#define PTAM            "/ptam.out"
#define PTAMSST         "/ptam.sst"
#define CODER           "/coder.out"
#define CODERSST        "/coder.sst"
#define MONITOR         "/ptruntime.o"

/* Assembler and Linker */
char *assembler =       "/usr/bin/gcc";
char *load =            "/usr/bin/gcc";
char *move =            "/bin/mv";

/* Option Flags */
#define true    1
#define false   0
int iOption =  false;
int IOption =  false;
int cOption =  false;
int Soption =  false;
int Toption =  false;
int oOption =  99;      /* number of compiler passes to run */

/* Arguments to the Processor */
int arg =  0;
char pnamechars[100];
char *progname =  pnamechars;
char progsuf;
char *sourceFile =  0;
int nPtcArgs =  0;
char *ptcArgs[15];

char loadFile[256];
char tCodeFile[256];
char asmFile[256];
char objFile[256];
char *linkFiles[10];
int nLinkFiles = 0;

char *tempdir =         "/tmp";
char optionFile[256], tempname[256], parseStream[256], semTempFile[256];
#define PARSESTREAM     ".parseout"
#define SEMTEMPFILE     ".semtemp"

FILE *optfd;
FILE *tmpfd;

/* Error Severity Levels */
#define fatal   1
#define continue  0

/* Routine declarations */
extern char * mkstemp();
extern char * strip();
extern char suffix();


main (argc, argv)
    char *argv[];
    int argc;
{ 
    int i;
    char c;

    arg = 0;

    fprintf (stderr, "PT Pascal v5.1 (c) 2021 Queen's University, (c) 1980 University of Toronto\n");

    if (argc == 1)  useerror ();

    /* Process Options */

    strcpy (optionFile, "/tmp/ptcXXXXXX");
    mkstemp (optionFile);
    if ((optfd=fopen(optionFile, "w")) == NULL)  
        error ("ptc: Unable to create ", optionFile, fatal);

    while (++arg < argc)  {

        if (*argv[arg] == '-')  {
            option (argv);

        } else {
            if (!sourceFile)  {
                sourceFile = argv[arg];
                strcpy (progname, sourceFile);
                progsuf = suffix (progname);
                progname = strip (progname);
            } else 
                linkFiles[nLinkFiles++] = argv[arg];
        }
    };

    fclose (optfd);

    if (!sourceFile)  useerror ();


    /* Run the Compiler */

    strcpy (loadFile, progname); strcat (loadFile, ".out");
    strcpy (tCodeFile, progname); strcat (tCodeFile, ".tc");
    strcpy (asmFile, progname); strcat (asmFile, ".s");
    strcpy (objFile, progname); strcat (objFile, ".o");

    strcpy (tempname, "/ptcXXXXXX");
    mkstemp (tempname);
    strcpy (parseStream, tempdir);
    strcat (parseStream, tempname);
    strcat (parseStream, PARSESTREAM);
    strcpy (semTempFile, tempdir);
    strcat (semTempFile, tempname);
    strcat (semTempFile, SEMTEMPFILE);

    strcpy (stdIds, library); strcat (stdIds, STDIDS);
    strcpy (scanSst, library); strcat (scanSst, SCANSST);
    strcpy (parseSst, library); strcat (parseSst, PARSESST);
    strcpy (parser, library); strcat (parser, PARSER);
    strcpy (semanSst, library); strcat (semanSst, SEMANSST);
    strcpy (semantic, library); strcat (semantic, SEMANTIC);
    strcpy (coderSst, library); strcat (coderSst, CODERSST);
    strcpy (coder, library); strcat (coder, CODER);
    strcpy (ptamSst, library); strcat (ptamSst, PTAMSST);
    strcpy (ptam, library); strcat (ptam, PTAM);
    strcpy (monitor, library); strcat (monitor, MONITOR);

    if (progsuf == 'p') {

        /* Compile PT Program to Assembly Code */

        ptcArgs[0] = "parser";
        ptcArgs[1] = stdIds;
        ptcArgs[2] = scanSst;
        ptcArgs[3] = parseSst;
        ptcArgs[4] = sourceFile;
        ptcArgs[5] = parseStream;
        ptcArgs[6] = optionFile;
        ptcArgs[7] = 0;

        if (callsys (parser, ptcArgs, 0, 0))  goto finished;

        /* If the parser aborted translation (indicated by an empty token
           stream) then exit.                                       */
        tmpfd = fopen(parseStream, "r");
        if (getc(tmpfd) == EOF)  goto finished;

        /* decrement twice, since scanner/parser is actually one pass */
        if (--oOption <= 0) goto finished;
        if (--oOption <= 0) goto finished;

        ptcArgs[0] = "semantic";
        ptcArgs[1] = semanSst;
        ptcArgs[2] = parseStream;
        ptcArgs[3] = semTempFile;
        ptcArgs[4] = tCodeFile;
        ptcArgs[5] = optionFile;
        ptcArgs[6] = 0;

        if (callsys (semantic, ptcArgs, 0, 0)) goto finished;

        /* If the semantic pass had a fatal error (first word > 0) then abort */
        tmpfd = fopen(tCodeFile, "r");
        fread(&i, 4, 1, tmpfd);
        if (i) goto finished;

        if (--oOption <= 0) goto finished;

        if (IOption) { 
            ptcArgs[0] = "ptam";
            ptcArgs[1] = ptamSst;
            ptcArgs[2] = tCodeFile;
            ptcArgs[3] = optionFile;
            ptcArgs[4] = 0;

            callsys (ptam, ptcArgs, 0, 0);
            goto finished;
        }

        if (! iOption) { 
            ptcArgs[0] = "coder";
            ptcArgs[1] = coderSst;
            ptcArgs[2] = tCodeFile;
            ptcArgs[3] = asmFile;
            ptcArgs[4] = optionFile;
            ptcArgs[5] = 0;

            if (callsys (coder, ptcArgs, 0, 0)) goto finished;

            progsuf = 's';
        }
    };

    if (iOption) goto finished;
    if (Soption) goto finished;

    if (progsuf == 's') {

        /* Assemble the PT program */
        ptcArgs[0] = "as";
        ptcArgs[1] = "-m32";
        ptcArgs[2] = "-c";
        ptcArgs[3] = "-o";;
        ptcArgs[4] = objFile;
        ptcArgs[5] = asmFile;
        ptcArgs[6] = 0;
            
        if (callsys (assembler, ptcArgs, 0, 0)) goto finished;

        progsuf = 'o';

        if (!Toption) unlink (asmFile);
    };

    if (cOption) goto finished;

    if (progsuf != 'o')  useerror();

    /* Create a load module */
    ptcArgs[0] = "cc";
    ptcArgs[1] = "-m32";
    ptcArgs[2] = "-w";
    ptcArgs[3] = monitor;
    ptcArgs[4] = objFile;

    nPtcArgs = 5;
    i = 1;
    while (i <= nLinkFiles)
        ptcArgs[nPtcArgs++] = linkFiles[i++];

    ptcArgs[nPtcArgs] = 0;

    if (callsys(load, ptcArgs, 0, 0)) goto finished;

    if (!Toption) unlink (objFile);

    /* Move a.out to progname.out */
    ptcArgs[0] = "mv";
    ptcArgs[1] = "a.out";
    ptcArgs[2] = loadFile;
    ptcArgs[3] = 0;

    callsys(move, ptcArgs, 0, 0);

finished:
    unlink (optionFile);
    if (!Toption) {
        unlink (parseStream); 
        unlink (semTempFile);
        if (!iOption) {
            unlink (tCodeFile);
        }
    }
    return(0);
}


option (argv)
    char *argv[];
{ 
    int j,value;
    char c;

    j = 0;

    while (argv[arg][++j])  {

        c = argv[arg][j];

        if (c == '-')  {
            value = false;
            j++;
            c = argv[arg][j];
        } else {
            value = true;
        };

        switch (c)  {
            case 'h':
                help ();
                unlink (optionFile);
                exit (0);
    
            case 'i':
                iOption = value;
                break;

            case 'I':
                IOption = value;
                break;

            case 'c':
                cOption = value;
                break;

            case 'S':
                Soption = value;
                break;

            case 'T':
                tempdir = ".";
                Toption = value;
                break;

            case 'L':
                library = argv[++arg];
                return(0);

            case 'm': case 'k': case 'O':
                if (value)  putc (c, optfd);
                break;

            case 'l':
                linkFiles[nLinkFiles++] = argv[arg];
                break;

            case 'o':
                putc (c, optfd);
                c = argv[arg][++j];
                putc (c, optfd);
                oOption = (c - '0');
                break;

            case 't':
                putc (c, optfd);
                c = argv[arg][++j];
                putc (c, optfd);
                break;

            case 'n':
                c = argv[arg][++j];
                putc (c, optfd);
                c = argv[arg][++j];
                putc (c, optfd);
                break;

            default:
                useerror ();
        }
    }
}


callsys (prog, args)
    char prog[], *args[];
{ 
    int t, status;

    if ((t=fork())==0) {
        execv (prog, args);
        error ("ptc: Unable to execute ", prog, fatal);

    } else {
        if (t == -1) 
            error ("ptc: Unable to fork ", prog, fatal);
    };

    while (t!=wait(&status));

    if ((t=(status&0377)) != 0 && t!=14) {
        if (t!=2)       /* interrupt */
            error ("ptc: Fatal processor error in ", prog, fatal);
    };

    return ((status>>8) & 0377);
}


error (msg, data, severity)
    char *msg, *data;
    int severity;
{
    fprintf (stderr, "%s%s\n", msg, data);

    if (severity == fatal)  {
        unlink (optionFile); 
        exit (10);
    };
}


useerror ()
{
    error ("Usage:  ", usage, fatal);
}


char * strip (as)
    char *as;
{ 
    register char *s;
    s = as;
    while (*s)
        if (*s++ == '/')
            as = s;

    while (--s > as)
        if (*s == '.')  {
            *s = '\0';
            return (as);
        };

    useerror ();
}



char suffix (as)
    char *as;
{ 
    register char *s;
    s = as;
    while (*s)
        s++;
    while (--s > as)
        if (*s == '.')  {
            s++;
            return (*s);
        };
    return ('\0');
}



help ()
{ 
    char c;
    printf ("'ptc' invokes the PT compiler to compile a PT source program.\n");
    printf ("The command syntax is:\n\n");
    printf ("   %s\n\n", usage);
    printf ("The input source program is assumed to be in progname.pt .\n");
    printf ("The UNIX load module for the program will be placed in progname.out .\n");
    printf ("The -i, -S and -c options specify PT tcode, UNIX assembly code or object code\n");
    printf ("respectively are to be placed in progname.tc, progname.s or progname.o instead.\n");
    printf ("The -O flag specifies that no line numbering and checking\n");
    printf ("code should be generated for the program.\n");
    printf ("This has the effect of reducing the size of the code generated\n");
    printf ("and speeding up execution, while sacrificing runtime checking.\n");
    printf ("Error messages are sent to the standard output.\n");
    printf ("Disasters (such as not finding the compiler) are logged\n");
    printf ("on the diagnostic output.\n\n");
    printf ("Option details? ");
    c = getchar();
    if (c == 'y')  {
        printf ("\nThe following options are recognized.\n\n");
        printf ("  -i     compile to PT tcode only, do not generate assembly code\n");
        printf ("  -I     compile to PT tcode and run it using the PT abstract machine\n");
        printf ("  -S     compile to assembly code only, do not assemble or link\n");
        printf ("  -c     compile to object code only, do not link\n");
        printf ("  -O     optimize generated code by sacrificing runtime line\n");
        printf ("               numbering and checking\n");
        printf ("  -nDD   use only the first DD (08 <= DD <= 50) characters to\n");
        printf ("               distinguish between different identifiers (default 50)\n");
        printf ("  -L DIR  run the version of the compiler passes in directory DIR\n");
        printf ("               instead of the production compiler in /usr/local/lib/pt\n");
        printf ("  -T     put all compiler temporary files in the present working directory\n");
        printf ("               and do not remove them when done\n");
        printf ("  -oN    execute only to end of compiler pass N\n");
        printf ("               (1 or 2 = scan/parse, 3 = semantic, 4 = coder)\n");
        printf ("  -tN    trace execution of compiler pass N\n");
        printf ("               (1 = scan, 2 = parse, 3 = semantic, 4 = coder)\n");
    }
}

