#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib2
import json

def github_get_list_resource_all(start_url):
    response = urllib2.urlopen(start_url)
#    In [8]: response.headers.getheader('Link')
#    Out[8]: '<https://api.github.com/repositories/4871047/stargazers?page=2>; rel="next", <https://api.github.com/repositories/4871047/stargazers?page=3>; rel="last"'
    link = response.headers.getheader('Link')
    ret = []
    ret.extend(json.loads(response.read()))
    for l in link.split(','):
        alink = l.split('>')[0][1:]
        what = l.split('"')[1]
        if what == 'next':
            ret.extend(github_get_list_resource_all(alink))
    return ret

def github_get_stargazers(user, repo):
    start_url = 'https://api.github.com/repos/%s/%s/stargazers' % (user, repo)
    return github_get_list_resource_all(start_url)

def main():
    import sys
    if len(sys.argv) == 3:
        user = sys.argv[1]
        repo = sys.argv[2]
    else:
        print "usage: {user} {repo}"
        sys.exit(-1)
    return github_get_stargazers(user, repo)


if __name__ == '__main__':
    r = main()
