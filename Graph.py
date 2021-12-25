import matplotlib.pyplot as plt
plt.style.use(['dark_background'])




file = open("../Graph_Results.txt");
contents = []
for line in file:
    contents.append(line)



x = [contents[0], contents[2]]
values = [int(contents[1]), int(contents[3])]

x_pos = [i for i, _ in enumerate(x)]

fig, ax = plt.subplots()
rects1 = ax.bar(x_pos, values, color= ('green', 'blue'))
plt.ylabel("Time Taken in Milliseconds")
plt.title("Comparision of Parallel and Serial Execution")
plt.xticks(x_pos, x)
def autolabel(rects):
    
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 0.5*height,
                '%d' % float(height),
        ha='center', va='bottom')
autolabel(rects1)

plt.show()


