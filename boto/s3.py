# Refs:
#    * http://boto.s3.amazonaws.com/s3_tut.html
#

from boto.s3.connection import S3Connection
from boto.s3.key import Key

conn = S3Connection()

# Bucket namespace is shared by all users
#bucket = conn.create_bucket('hpl-test')
bucket = conn.get_bucket('hpl-test')
#testdir = 'test'
testfile = __file__

k = Key(bucket)
k.key = 'test/s3.py'

print 'Uplaad'
#k.set_contents_from_string('This is a test of S3')
k.set_contents_from_filename(testfile)

print 'Download'
k.get_contents_to_filename(testfile + '.down')
print k.get_contents_as_string()
