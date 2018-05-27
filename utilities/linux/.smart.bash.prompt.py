import os
from socket import gethostname
hostname = gethostname()
username = os.environ['USER']
pwd = os.getcwd()
homedir = os.path.expanduser('~')
pwd = pwd.replace(homedir, '~', 1)
# first 10 chars+last 20 chars
if len(pwd) > 28:
    pwd = ''.join([username, ':', pwd[:5], '...', pwd[-20:], '$ ']) 
#print '[%s@%s:%s] ' % (username, hostname, pwd)
#print '[%s:%s] ' % (username, pwd)
print pwd
