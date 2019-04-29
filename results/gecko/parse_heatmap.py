
import os,sys,glob


def parse_stream_output(lines):
	for l in lines:
		if "Bandwidth (GB/s):" in l:
			bw = float(l.split()[-1])
			return bw
	return None

if len(sys.argv) < 2:
	print "Please provide a folder to proceed."
	exit(1)

folder = sys.argv[1]

hc_list = [0.01, 0.1, 0.5, 1, 5] + range(10, 99, 10)
hc_list = [float(v) for v in hc_list]


values = dict()
for hc in hc_list:
	values[hc] = [0,0,0,0]

gpu_count_set = set()

file_list = glob.glob(folder+"/*")

for filename in file_list:
	f = open(filename, "r")
	lines = f.readlines()
	f.close()

	bw = parse_stream_output(lines)
	if bw is None:
		continue

	components = filename.split("-")
	if len(components) < 3:
		print "Error in file:", filename, components
		continue

	gpu_count = int(components[-2])
	hc = float(components[-1])

	gpu_count_set.add(gpu_count)

	if hc not in hc_list:
		continue

	values[hc][gpu_count-1] = bw


hc_list = reversed(hc_list)
for hc in hc_list:
	bw_list = values[hc]
	gpu_count = len(gpu_count_set)
	bw_list = bw_list[:gpu_count]
	print "%.2f: " % (hc),
	for i, bw in enumerate(bw_list):
		if i == len(bw_list)-1:
			print "%.2f" % (bw),
		else:
			print "%.2f," % (bw),
	print ""

