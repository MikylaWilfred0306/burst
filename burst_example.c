% ls
data.txt
% wc --lines data.txt
872
% ./burst -a data.txt
% ls
data.txt data-0.txt data-1.txt
% wc --lines data-0.txt
500
% wc --lines data-1.txt
372
% cat data-1.txt data-2.txt | diff - data.txt
