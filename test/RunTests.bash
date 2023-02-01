#! /bin/bash

for test in `ls | grep 'test$'` 
do
   ./$test
done

