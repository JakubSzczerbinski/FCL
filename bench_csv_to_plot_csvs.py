

import csv
import json

names = []
iterations = []
real_time = []
cpu_time = []

def write_csv(file_path, data):
	with open(file_path, 'w') as file:
	    spamwriter = csv.writer(file, delimiter=',',
                            quotechar='|', quoting=csv.QUOTE_MINIMAL)
	    print(names)
	    print(data)
	    spamwriter.writerow(names)
	    spamwriter.writerow(data)


with open('bench.json', 'r') as bench_json:
	bench_reader = json.load(bench_json);
	for bench in bench_reader['benchmarks']:
		print (bench)
		names.append(bench['name'])
		iterations.append(bench['iterations'])
		real_time.append(bench['real_time'])
		cpu_time.append(bench['cpu_time'])

	write_csv('cpu_time.csv', cpu_time)
	write_csv('real_time.csv', real_time)
	write_csv('iterations.csv', iterations)

