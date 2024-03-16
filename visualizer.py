import matplotlib.pyplot as plt


# Question A
# data = {
#     'Terminal 1': {'max': 69, 'avg': 41.557},
#     'Terminal 2': {'max': 41, 'avg': 15.913},
#     'Car Rental': {'max': 49, 'avg': 16.839}
# }

# Question B
# data = {
#     'Terminal 1': {'max': 315.007, 'avg': 179.385},
#     'Terminal 2': {'max': 227.734, 'avg': 90.388},
#     'Car Rental': {'max': 125.339, 'avg': 43.040}
# }

# Question C
# data = {
#     'max': {'': 20},
#     'average': {'': 19.537}
# }

# Question D
# data = {
#     'Terminal 1': {'max': 15.776, 'avg': 8.086, 'min':4.976},
#     'Terminal 2': {'max': 20.367, 'avg': 7.473, 'min':1.493},
#     'Car Rental': {'max': 29.154, 'avg': 15.000, 'min':10.304}
# }

# Question E
data = {
    'max': {'': 62.963},
    'average': {'': 50.573},
    'min': {'': 46.020}
}

# Question F
# data = {
#     'Terminal 1': {'max': 339.411, 'avg': 201.891, 'min':21.946},
#     'Terminal 2': {'max': 243.302, 'avg': 107.608, 'min':16.024},
#     'Car Rental': {'max': 153.225, 'avg': 63.029, 'min':13.437}
# }


terminals = list(data.keys())
attributes = list(data[terminals[0]].keys())
colors = ['red', 'orange', 'yellow']

fig, ax = plt.subplots()
bar_width = 0.2
index = range(len(attributes))
plotted_terminals = {}

for i, terminal in enumerate(terminals):
    terminal_data = data[terminal]
    for j, attr in enumerate(attributes):
        value = terminal_data[attr]
        if terminal not in plotted_terminals:
            ax.bar(index[j] + i * bar_width, value, bar_width, color=colors[i], label=terminal)
            plotted_terminals[terminal] = True
        else:
            ax.bar(index[j] + i * bar_width, value, bar_width, color=colors[i])

# Question A
# ax.set_title('Average and maximum number in each queue')

# Question B
# ax.set_title('Average and maximum delay in each queue')

# Question C
# ax.set_title('Average and maximum number on the bus')

# Question D
# ax.set_title('Average, maximum, and minimum, time the bust stopped in each location')

# Question E
ax.set_title('Average, maximum, and minimum the bus to make a loop')

# Question F
# ax.set_title('Average, maximum, and minimum time person is in the system')

ax.set_xlabel('Attribute')
ax.set_ylabel('Value')
ax.set_xticks([i + (len(terminals) - 1) * bar_width / 2 for i in index])
ax.set_xticklabels(attributes)
ax.legend()

plt.show()
