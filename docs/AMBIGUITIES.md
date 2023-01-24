This is a list of abiguities and how we have resolved them.

# Can a comment end not with newline but with end of file?
- We said no, and we throw an error if that occurs
- This was chosen as that was the old behaviour from PT and a change was not specified
