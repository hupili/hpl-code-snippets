# Refs:
#    * http://boto.s3.amazonaws.com/s3_tut.html
#
# Download everything from a bucket.
#
# Step 1: Create new IAM user and authorise read permission
#         to the bucket you want to archive
#
# Step 2: Write the followings in your ~/.boto file
#
#[Credentials]
#aws_access_key_id =
#aws_secret_access_key =
#
# Step 3: Run this script like `python down-s3.py <my-bucket-name>`
#

import os
import sys
from boto.s3.connection import S3Connection
from boto.s3.key import Key

DIR_ROOT = 'archive'

conn = S3Connection()

# Get the bucket name.
# Note, bucket namespace is shared by all users
bucket = conn.get_bucket(sys.argv[1])

keys = list(bucket.list())
print 'Total:', len(keys), 'files'
for key in keys:
    print key
    if key.size == 0 and key.name.endswith('/'):
        # Probably a dir, ignore
        # http://stackoverflow.com/questions/9954521/s3-boto-list-keys-sometimes-returns-directory-key
        print key, 'Ignored. Probably a dir created in web console'
        continue
    # Immediate feedback if something goes wrong
    sys.stdout.flush()
    sys.stderr.flush()
    dest_path = os.path.join(DIR_ROOT, key.name)
    try:
        os.makedirs(os.path.dirname(dest_path))
    except Exception as e:
        if e.errno == 17:
            # Dir exists. Don't worry
            pass
        else:
            raise e
    key.get_contents_to_filename(dest_path)

