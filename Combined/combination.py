import sys
import itertools


file = open("combination.txt")

words = []
for line in file:
    if line not in words:
        words.append(line[0:len(line)-1])


file.close()
    

all_combinations = list(itertools.combinations(words[1:],int(words[0])))
file = open("Results.txt","w")

for pairs in range (len(all_combinations)):
    c = ""
    for word in all_combinations[pairs]:
        c += word
        c += ","
    if (pairs == len(all_combinations)-1):
        file.write(c[0:len(c)-1])
    else:
        file.write(c[0:len(c)-1]+"\n") 
    
file.close()   