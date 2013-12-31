# Refs:
#   * http://www.withoutthesarcasm.com/using-amazon-glacier-for-personal-backups/
#   * http://docs.aws.amazon.com/amazonglacier/latest/dev/api-initiate-job-post.html

# The workflow of Glacier
#
# Upload:
#    * Upload directly, see test_upload()
# Retrieve:
#    * Issue jobs. See test_retrieval() and test_list_inventory()
#      for two types of jobs.
#    * When jobs complete, e.g. notified via SNS, Get the job output.
#      See test_list_jobs_and_get_output()

from boto.glacier.layer1 import Layer1
from boto.glacier.vault import Vault
from boto.glacier.concurrent import ConcurrentUploader
import sys
import os.path

import json
import sh

target_vault_name = 'test'
fname = __file__

# Use credentials directly:
# access_key_id = '...'
# secret_key = '...'
# glacier_layer1 = Layer1(aws_access_key_id=access_key_id, aws_secret_access_key=secret_key)

# Recommend to put your credentials in ~/.boto:
# [Credentials]
# aws_access_key_id = <your access key>
# aws_secret_access_key = <your secret key>

# region_name is not necessary
glacier_layer1 = Layer1(region_name='us-west-2')

def test_upload():
    print("create vault")
    glacier_layer1.create_vault(target_vault_name)
    print("start to upload")
    uploader = ConcurrentUploader(glacier_layer1, target_vault_name, 32*1024*1024)
    archive_id = uploader.upload(fname, fname)
    print("upload success! archive id: '%s'"%(archive_id))
    return archive_id

def test_list_inventory():
    # http://docs.aws.amazon.com/amazonglacier/latest/dev/api-initiate-job-post.html
    job_id = glacier_layer1.initiate_job(target_vault_name,
                                         {"Description":"inventory-job",
                                          "Type":"inventory-retrieval",
                                          "Format":"JSON"})
    print("job ID:" + str(job_id))
    return job_id

def test_retrieval(archive_id):
    # http://docs.aws.amazon.com/amazonglacier/latest/dev/api-initiate-job-post.html
    job_id = glacier_layer1.initiate_job(target_vault_name,
                                         {"Description":"archive-job",
                                          "Type":"archive-retrieval",
                                          "ArchiveId": archive_id})
    print("job ID:" + str(job_id))
    return job_id

def test_list_jobs():
    out = glacier_layer1.list_jobs(target_vault_name)
    print json.dumps(out, indent=2)
    return out

def test_list_jobs_and_get_output():
    sh.mkdir('-p', 'glacier-output')
    jobs = test_list_jobs()
    for j in jobs['JobList']:
        #print '------'
        #print json.dumps(j, indent=2)
        if j['Completed'] == True:
            print 'saving job data:', j['JobId']
            #data = 'minick output for testing purpose'
            response = glacier_layer1.get_job_output(target_vault_name, j['JobId'])
            with open('glacier-output/%s' % j['JobId'], 'w') as fp:
                fp.write(response.read())

def test_delete_archive(archive_id):
    response = glacier_layer1.delete_archive(target_vault_name, archive_id)
    print response
    return response


if __name__ == '__main__':
    # Comment/Uncomment those functions for test

    #archive_id = test_upload()
    #test_list_inventory()
    #test_retrieval()
    jobs = test_list_jobs()
    #test_list_jobs_and_get_output()

    #test_delete_archive('tuPWFvJznbFVuAFZHKha_Fkckoc7tBzyAzqWkWcV4MnWWtyXax8QBrABkOik7c_0IfBo64eEsoPCd3a-qkfJba3UsiimlTNhk5w35HS1HWnipwmxvRqkiBSe3ife0vk0VXm7e7BymA')
