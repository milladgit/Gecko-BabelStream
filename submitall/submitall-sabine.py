
import os,sys

job_template = '''#!/bin/bash

### Set the job name
#SBATCH -J %s

#SBATCH -p gpu

### Specify the number of cpus for your job.
#SBATCH -N 1                 # total number of nodes
#SBATCH --exclusive



module load CUDA/10.0.130
module load pgi/18.7-64-bit

export GECKO_CONFIG_FILE=%s
export GECKO_POLICY=\"%s\"

cd %s
rm -f %s

'''

gecko_config_folder_base = "/home/mghane2/gecko-configs/"
stream_base_folder = "/home/mghane2/gecko-experiments/Gecko-BabelStream/" 


app_list = ['gecko-stream']
iteration_count = 1


for gpu_count in range(1, 3):
	#for hc in range(10, 99, 10):
	for hc in [5.0,1.0,0.5,0.1,0.01]:
		config_name = "h1g%d" % (gpu_count)
		config_file = gecko_config_folder_base + config_name
		percent = "percentage:[%.2f" % (1.0*hc)

		rest = 100 - hc
		delta = 1.0 * rest / gpu_count
		i = 0
		while i < gpu_count:
			if i == gpu_count-1:
				r = 100.0 - (gpu_count-1)*delta - hc
				percent += ",%.2f" % (r)
			else:
				percent += ",%.2f" % (delta)
			i += 1

		percent += "]"


		for app in app_list:
			result_file = "result-%s-%d-%.2f" % (app, gpu_count, hc)
			job_filename = "job-%s-%d-%.2f" % (app, gpu_count, hc)
			app_full = stream_base_folder
			job_text = job_template % ("%s-%d-%.2f" % (app, gpu_count, hc),config_file, percent, app_full, result_file)
			for i in range(iteration_count):
				job_text += "./gecko-stream --triad-only -n 200 -s 256000000 &> %s \n" % (result_file)
				if(i < iteration_count-1):
					job_text += "sleep 3 \n"

			job_text += "\n\n"
			
			f = open(job_filename, "w")
			f.write(job_text)
			f.close()

			print "Running %s ..." % (job_filename)

			os.system("sh %s" % (job_filename))
			#os.system("sbatch %s" % (job_filename))





