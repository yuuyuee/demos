import sys
import xlrd

def Main(filename):
  buf = None
  with open(filename, 'rb') as fd:
    buf = fd.read()

  book = xlrd.open_workbook(file_contents = buf)
  sh = book.sheet_by_index(0)
  print(sh)

if __name__ == '__main__':
  if len(sys.argv) != 2:
    print('{} filename'.format(sys.argv[0]))
  else:
    Main(sys.argv[1])
