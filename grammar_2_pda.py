import sys

'''For storing transition functions for pda'''
transition_functions = {}

def Print(s):
	'''For printing'''
	return sys.stdout.write(str(s))


def detect_non_terminals_And_form_transition_function(prod):
	'''Returning Non-terminals associated with current production and transforming
	into transition function'''
	lhs, rhs = prod.split(':')
	rhs_productions = rhs.split('|')

	temp_list = set()
	ret_nts = []

	'''Printing the transition function'''
	Print(chr(948)+"(q,"+chr(949)+","+lhs+") |- "+"{")

	for index in range(len(rhs_productions)):
		el = rhs_productions[index]
		# Evaluating transitions
		temp_list.add(('q',el))
		# printing Transitions
		if index == 0:
			Print("(q"+","+el+")")
		else:
			Print(", (q"+","+el+")")
		# Detecting non_terminals
		for j in range(len(el)):
			if el[j].islower():
				ret_nts.append(el[j])

	print("}")
	# Storing one transition
	transition_functions[('q',chr(949),lhs)] = temp_list

	return ret_nts


def handle_epsilon_productions(nts):
	'''Docstring of this function'''
	temp_list = set()

	for elem in nts:
		# Printing epsilon productions
		print(chr(948)+"(q,"+elem+","+elem+") |- {(q,"+chr(949)+")}")
		temp_list.add(('q',chr(949)))
		# Storing transition
		transition_functions[('q',elem,elem)] = temp_list


if __name__ == "__main__":

	filename = input("\nEnter file name : ")
	# Reading line by line, where each line indicates one production
	with open("cfg_sample_files/"+filename) as f:
	    grammar = f.readlines()

	# Removing unnecessary white spaces and '\n's
	grammar = [x.strip() for x in grammar]

	print("\nGiven CFG : \n-------------------")
	for el in grammar:
		print(el)

	non_terminals = []

	print("-------------------\n\nConverted PDA :\n--------------------------------------")

	# Evaluating transition functions for pda
	for production in grammar:
		for elem in detect_non_terminals_And_form_transition_function(production):
			if elem not in non_terminals:
				non_terminals.append(elem)

	handle_epsilon_productions(non_terminals)

	print("--------------------------------------\n")

	# Save the pda to a text file
	f = open("After_cfg_to_pda_sample_files/cfg_2_pda@"+filename+"@.txt",'w')
	f.write(str(transition_functions))
	f.close()

	# Read back from the file
	# f = open("After_cfg_to_pda_sample_files/cfg_2_pda@"+filename+"@.txt",'r')
	# data=f.read()
	# f.close()
	# pda_dict = eval(data)