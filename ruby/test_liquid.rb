gem 'liquid', '=2.4.0'
require 'Liquid.rb'
body=<<eos
Truncate {{text | truncate: 6}}
eos
@template = Liquid::Template.parse(body)
puts @template.render("text" => "I'm testing Liquid")
puts @template.render("text" => "乱石穿空，惊涛拍岸")

body=<<eos
Truncate {{text | truncatewords: 5}}
eos
@template = Liquid::Template.parse(body)
puts @template.render("text" => "I'm testing the word truncation of Liquid")
puts @template.render("text" => "测试测试测试测试")

# Test on liquid (2.3.0)
# Truncate I'...
# Truncate 测试...
# Truncate I'm testing the word truncation...
# Truncate 测试测试测试测试
# [ok]
#
# Test on liquid (2.5.0)
# Truncate I'...
# Truncate 测试...
# Truncate I'm testing the word truncation...
# Truncate 测试测试测试测试
# [ok]

exit
# Init test: an executable minimum example 
body=<<eos
hi {{text}}
eos
@template = Liquid::Template.parse(body)
puts @template.render("text" => "I'm testing Liquid")
