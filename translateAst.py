import sys

if len(sys.argv) != 2:
  print "no filename given"
  sys.exit()
#print sys.argv
filename = sys.argv[1]
start = True
skip = False
convD = {}

file = open(filename.split('.')[0]+"_cleansed.txt", 'w+')

with open(filename, 'r') as f:
  counter = 0
  for line in f:
    counter += 1
    if start:
      if skip and not line.startswith("TranslationUnitDecl"):
        continue
      elif skip:
        skip = False
      if line.startswith("clang"):
        continue
      elif line.startswith("In file"):
        skip = True
      else:
        start=False
        convD[line.split(' ')[0]] = 0
        pos = line.find('>')
        label = ""
        if pos > -1:
          label = line[pos:]
        file.write(str(0)+":"+label+"\n")
    else:
      if skip and not line.startswith("TranslationUnitDecl"):
        continue
      elif skip:
        skip = False
      if line.startswith("In file") or line.startswith("Down"):
        skip = True
        continue
      else:
        stripped = line.lstrip('| `')
        indent = len(line)-len(stripped)
        if indent!=0 and not stripped.startswith('-'):
          continue
        elif indent==0 and not line.startswith("TranslationUnitDecl"):
          #print "indent==0: " + line + ", counter=" + str(counter)
          continue
        elif indent!=0:
          stripped = stripped[1:]
          indent +=1
        name = stripped.split(' ')[0]
        try:
          num = convD[name]
        except KeyError:
          convD[name] = len(convD)
          #print name
          #print counter
          num = len(convD)
        pos = stripped.find('>')
        label = ""
        if pos > -1:
          label = stripped[pos:].lstrip('> ')
        file.write(" "*indent + str(num)+":"+label)
file.close()
