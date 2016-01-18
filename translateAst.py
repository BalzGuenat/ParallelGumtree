import sys

if len(sys.argv) != 2:
  print "no filename given"
  sys.exit()
#print sys.argv
filename = sys.argv[1]
start = True
skip = False
convD = {"artificial_root":0}

file = open(filename.split('.')[0]+".test", 'w+')
file.write("0:artificial_root\n")
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
        convD[line.split(' ',1)[0]] = 0
        pos = line.find('>')
        label = ""
        if pos > -1:
          label = line[pos:].lstrip('> ').strip()
        file.write(" 1:"+label+"\n")
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
        indent = len(line)+2-len(stripped)
        if indent!=2 and not stripped.startswith('-'):
          continue
        elif indent==2 and not line.startswith("TranslationUnitDecl"):
          #print "indent==0: " + line + ", counter=" + str(counter)
          continue
        elif indent!=2:
          stripped = stripped[1:]
          indent +=1
        name = stripped.split(' ',1)[0]
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
          label = stripped[pos:].lstrip('> ').strip()
          if label.startswith("col") or label.startswith("line"):
            try:
              label = label.split(' ',1)[1]
            except IndexError:
              pass
        file.write(" "*indent/2 + str(num)+":"+label+"\n")
file.close()
