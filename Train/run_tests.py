#!/usr/bin/env python

import os
import subprocess

i = 1
passed = 0
failed = 0
while True:
  input_name = "testinput" + str(i) + ".txt"
  output_name = "testoutput" + str(i) + ".txt"
  if not(os.path.exists(input_name)):
    break
  if not(os.path.exists(output_name)):
    print("Test file input " + input_name + "exists, but not " + output_name)

  expect = open(output_name).read()
  input_file = open(input_name)
  p = subprocess.Popen(["java", "TrainMain"],
      stdout=subprocess.PIPE, # cause stdout to pipe when we use communicate
      stderr=subprocess.PIPE, # cause stderr to pipe when we use communicate
      stdin=input_file)
  (out, err) = p.communicate()

  if(filter(lambda x: x not in [">", " ", "\n"], list(err)) != []):
    print("There were errors running the test " + input_name)
    print(err)

  elif(not(expect.strip() == out.strip())):
    print("Test " + input_name + " failed, expected:")
    print(expect.strip())
    print("but got:")
    print(out.strip())
    failed += 1

    if (expect.strip() != expect) or (out.strip() != out):
      print "Note: leading and trailing whitespace was trimmed"

  else:
    print("Success for test " + input_name)
    passed += 1

  i += 1

print("%s tests passed, %s failed" % (passed, failed))


