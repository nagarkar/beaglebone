import os
from socket import gethostname
hostname = gethostname()
username = os.environ['USER']
pwd = os.getcwd()
homedir = os.path.expanduser('~')
pwd = pwd.replace(homedir, '~', 1)
if len(pwd) > 28:
    pwd = pwd[:5]+'...'+pwd[-20:] # first 10 chars+last 20 chars
#print '[%s@%s:%s] ' % (username, hostname, pwd)
print '[%s:%s] ' % (username, pwd)
