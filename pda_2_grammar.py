import sys

# Global variables
all_production_cfg = {}
terminals_for_cfg = {}
right = []
stack_base_symbol = 0
start_state = 0


def Print(s):
	'''For printing'''
	return sys.stdout.write(str(s))


def print_the_cfg():
	lhs_keys = (all_production_cfg.keys())
	lhs_keys = sorted(lhs_keys)
	lhs_keys.pop(lhs_keys.index('S'))
	lhs_keys.insert(0,'S')

	for lhs_k in lhs_keys:
		Print(lhs_k+":")

		for i in range(len(all_production_cfg[lhs_k])):
			if i == 0:
				Print(all_production_cfg[lhs_k][i])
			else:
				Print("|"+all_production_cfg[lhs_k][i])
		print()



def add_to_production(l):
	'''Storing a single production to the collection of production'''
	temp_right = []

	if right[0] != chr(949):
		temp_right.append(right[0])
	elif len(right) == 1:
		temp_right.append(right[0])

	for i in range(1,len(right)):
		temp_right.append(terminals_for_cfg[tuple(right[i])])
	rhs_production = ''.join(temp_right)

	if all_production_cfg.get(l,-1) == -1:
		all_production_cfg[l] = [rhs_production]
	else:
		temp_rhs_production = all_production_cfg[l]
		temp_rhs_production.append(rhs_production)
		all_production_cfg[l] = temp_rhs_production


def recursive_for_loop(n, state, index, left):
	'''Creating n-nested for loops'''
	if n==0:
		add_to_production(left)
	elif n==1:
		for ele in state:
			right[index][2] = right[index+1][0] = ele
			add_to_production(left)
	else:
		for ele in state:
			right[index][2] = right[index+1][0] = ele
			recursive_for_loop(n-1,state,index+1,left)


def generate_start_state_production(states, stack_symbols):
	left = 'S'
	temp_right = []

	for alls in states:
		temp_right.append(terminals_for_cfg[(start_state,stack_base_symbol,alls)])

	all_production_cfg[left] = temp_right


def generate_productions_for_cfg(transition, states):
	'''Generate all the productions for a single transition function'''
	lhs, rhs = transition.split("=")
	lhs = (lhs[2:-1]).split(",")
	rhs = (rhs[1:-1]).split(",")

	if len(right) != 0:
		right.clear()

	# Non-erasing production
	if rhs[1] != chr(949):

		right.append(lhs[1])
		right.append([rhs[0],(rhs[1])[0],'null'])
		for j in range(1,len(rhs[1])):
			right.append(['null',(rhs[1])[j], 'null'])

		# Considering all the combiantions of states by a recursive for loop
		for el1 in states:
			left = terminals_for_cfg[(lhs[0],lhs[2],el1)]
			(right[-1])[2] = el1
			recursive_for_loop(len(right)-2,states,1,left)
	# Erasing production
	else:
		left = terminals_for_cfg[(lhs[0], lhs[2], rhs[0])]
		right.append(lhs[1])
		add_to_production(left)


if __name__ == "__main__":

	filename = input("\nEnter file name : ")
	# Reading line by line, where each line indicates one production
	with open("pda_sample_files/"+filename) as f:
	    pda = f.readlines()

	# Removing unnecessary white spaces and '\n's
	pda = [x.strip() for x in pda]

	states = pda[0].split(",")
	stack_symbols = pda[1].split(",")
	start_state, stack_base_symbol = pda[2].split(",")

	terminal_start = ord('A')

	for el1 in states:
		for el2 in stack_symbols:
			for el3 in states:
				terminals_for_cfg[(el1,el2,el3)] = chr(terminal_start)
				terminal_start = terminal_start + 1

	print("\nGiven PDA : \n-------------------------")
	for i in range(3,len(pda)):
		# generating productions for a single transition function
		print(pda[i])
		generate_productions_for_cfg(pda[i], states)
	print("-------------------------\n")
	# Creating S-productions
	generate_start_state_production(states, stack_symbols)

	# Printing the pda
	print("Converted to CFG : \n-------------------------")
	print_the_cfg()
	print("-------------------------\n")

	# Saving into file
	f = open("After_pda_to_cfg_sample_files/pda_to_cfg@"+filename+"@.txt",'w')
	f.write(str(all_production_cfg))
	f.close()

	# Read back from the file
	# f = open("After_pda_to_cfg_sample_files/pda_to_cfg@"+filename+"@.txt",'r')
	# data=f.read()
	# f.close()
	# pda_dict = eval(data)