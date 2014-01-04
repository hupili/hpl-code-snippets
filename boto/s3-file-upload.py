# Refs:
#    * http://boto.s3.amazonaws.com/s3_tut.html
#

"""
Usage:
    s3-file-upload.py <bucket> <filename>

Options:
    -h --help     Show this screen.
"""

from boto.s3.connection import S3Connection
from boto.s3.key import Key

import docopt

def s3_upload_from_file(bucket, filename):
    conn = S3Connection()

    bucket = conn.get_bucket(bucket)

    k = Key(bucket)
    k.key = filename

    print 'Uplaading %s to bucket %s' % (filename, bucket)
    return k.set_contents_from_filename(filename)

if __name__ == '__main__':
    from docopt import docopt
    arguments = docopt(__doc__, version='s3-file-upload.py 0.1')
    bucket = arguments['<bucket>']
    filename = arguments['<filename>']
    print s3_upload_from_file(bucket, filename)

