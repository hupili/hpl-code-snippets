# Coupon Collector

## Result

```
$python simu.py
0.0
2477.85114885
2500.86006997
2495.997001
2500.34516371
2502.9520096
2508.20513248
2509.53520926
```

The result stablizes at about a little more than `n ln n`.
It is know that coupon collector has a sharp threshold at `t = n ln n + cn`.
In other words, for less than `t` steps, it is almost sure you can not collect all coupons; 
for more than `t` steps, it is almost sure you get all the coupons. 
The distribution concetrate at the mean (n ln n) very sharply. 

## Background

Yijing has 64 gua and each gua has 6 yao. 
Altogether, there are 384 different combinations. 
Last summer (July 2012), I discussed with one of my friend in FDU on 
how many trials should the author spend on obtaining all cases...
The legend of Yijing is that: 

   * Someone (priest?) augurs by drawing a case from those combinations. 
   * He/she then record what happens when this case comes up. 
   * Later, the users of the book augurs by drawing a case from the 384 combinations
   and lookup the book to predict what will happen.

Of course, Yijing is not restricted to this legendary use
(although there are still many such users).
Before going further, we are interested in how many time does it take for the author to write this book. 
