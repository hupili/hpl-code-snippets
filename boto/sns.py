# Refs:
#    * http://ridingpython.blogspot.hk/2011/11/aws-sns-how-to-send-out-messages-to-e.html

import boto
from boto import sns

# region is not necessary
#conn = boto.connect_sns(region_name='us-west-2')

#
conn = sns.connect_to_region('us-west-2')

# create a topic that endwith 'test' on SNS first.
all_topics = conn.get_all_topics()

# Sample response of topic list
# {u'ListTopicsResponse':
# {u'ResponseMetadata': {u'RequestId': u'xxxxx'},
#        u'ListTopicsResult':
#        {u'Topics': [{u'TopicArn': u'arn:aws:sns:us-west-2:xxxxx:test'}], u'NextToken': None}}}

_test_topics = filter(lambda x: x.endswith('test'), [t['TopicArn'] for t in all_topics['ListTopicsResponse']['ListTopicsResult']['Topics']])
test_topic = _test_topics[0]
print test_topic

msg = 'This is a test message'
sub = 'Test subject'
ret = conn.publish(test_topic, msg, sub)
print ret
