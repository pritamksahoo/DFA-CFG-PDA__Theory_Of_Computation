#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unordered_map>
#include <unordered_set>
using namespace std;
#define MAX_ALPHA 100

typedef enum{FAILURE=0,SUCCESS} Status;

typedef struct DFA_State_Tag
{
	int state_num;
	bool final;
	int* transitions;
}DFA_State;

std::unordered_map<int,char> map_state; 
std::unordered_set<char> st;

bool* reachable;

char get_cstate()
{
	static char ch='A'-1;
	ch = ch + 1;
	return ch;

}


void initialize_states(DFA_State* states_info,int num_states,int num_alpha)
{
	for (int i = 0; i < num_states; ++i)
	{
		states_info[i].transitions=(int*)malloc(sizeof(int)*num_alpha);
	}
}


Status input_state_info(DFA_State* state,int num_alpha,char alphabets[])
{
	assert(state != NULL);

	Status sc=SUCCESS;

	for (int i = 0; i < num_alpha; ++i)
	{
		int num;
		printf("Enter the transition on symbol %c\n",alphabets[i]);
		scanf("%d",&num);
		state->transitions[i]=num;
	}

	printf("Is this state a final state (1-Y/0-N)?\n");
	int ch;
	scanf("%d",&ch);

	if(ch == 1)
	{
		state->final=true;
	}
	else
	{
		state->final=false;
	}
	return sc;
}


Status input_dfa(DFA_State** states_info_ret,int* alpha_number,int* final_number,int* states_number,char alphabets[])
{
	(*alpha_number)=(*final_number)=(*states_number)=0;
	Status sc= SUCCESS;
	
	int num_alpha,num_states;
	int num_final_states;
	char ch;
	printf("Enter the number of alphabets/symbols in the language\n");
	scanf("%d",&num_alpha);
	printf("Enter alphabets\n");
	fflush(stdin);
	fflush(stdout);

	for (int i = 0; i < num_alpha; ++i)
	{
		scanf(" %c",&alphabets[i]);
	}
	
	printf("Enter the total number of states(Includes dummy states)\n");
	scanf("%d",&num_states);
	printf("Enter the number of final states\n");
	scanf("%d",&num_final_states);

	if(num_alpha < 1 or num_states < 1 or num_final_states < 1 or num_final_states > num_states)
	{
		sc = FAILURE;
	}
	else
	{
		(*states_info_ret)=(DFA_State*)malloc(sizeof(DFA_State)*num_states);
		DFA_State* states_info=(*states_info_ret);

		initialize_states(states_info,num_states,num_alpha);
		assert(states_info!=NULL);
		printf("RULES:\nThere are %d states numbered 0 to %d, each having transition on all input symbols.\n",num_states,num_states-1);

		printf("Enter states information\n");
		printf("Enter start state (0th state) information\n");

		sc=input_state_info(&states_info[0],num_alpha,alphabets);
		if(sc == SUCCESS)
		{
			states_info[0].state_num = 0;
			int count_final=0;
			if(states_info[0].final)
				count_final++;
			for (int i = 1; i < num_states and sc==SUCCESS; ++i)
			{
				printf("Enter information for state number %d\n",i);
				sc=input_state_info(&states_info[i],num_alpha,alphabets);

				states_info[i].state_num = i;
				if(states_info[i].final)
					count_final++;
			}
			if(sc == SUCCESS and count_final != num_final_states)
				sc=FAILURE;
		}

	}
	if(sc)
	{
		(*alpha_number)=num_alpha;
		(*final_number)=num_final_states;
		(*states_number)=num_states;
	}
	return sc;

}


void display_dfa(DFA_State* dfa,int num_states,int num_alpha,char alphabets[],bool consider_reach)
{
	printf("State Number\tSymbol Number\tTransition State\n");
	for (int i = 0; i < num_states; ++i)
	{
		bool pri = false;
		if(!(consider_reach && reachable[dfa[i].state_num]==false))
		{
			if(map_state.find(dfa[i].state_num) != map_state.end())
			{
				if((st.find(map_state[dfa[i].state_num]) == st.end()))
				{
					printf("%c", map_state[dfa[i].state_num]);
					pri = true;
					st.insert(map_state[dfa[i].state_num]);
				}
			}
			else
				printf("%d",dfa[i].state_num);
			if(dfa[i].final and pri){
				printf("*");
			}
			if(pri){
				for (int j = 0; j < num_alpha; ++j)
				{
					printf("\t\t%c\t\t",alphabets[j]);
					if(map_state.find(dfa[i].transitions[j]) != map_state.end())
						printf("%c\n", map_state[dfa[i].transitions[j]]);
					else
						printf("%d\n",dfa[i].transitions[j]);
					
				}
				printf("\n");
			}
		}
		
	}
}


void segregate_final_and_non_final(DFA_State dfa[],int num_states,int num_alpha,DFA_State final[],int num_final_states,DFA_State non_final[])
{
	//initialize 
	int fin_index=0,non_fin_index=0;
	initialize_states(final,num_final_states,num_alpha);
	initialize_states(non_final,num_states - num_final_states,num_alpha);
	//copy
	for (int i = 0; i < num_states; ++i)
	{
		if(dfa[i].final)
		{
			final[fin_index].final = true;
			final[fin_index].state_num = dfa[i].state_num;
			for(int j=0;j<num_alpha;++j){
				final[fin_index].transitions[j] = dfa[i].transitions[j];
			}
			fin_index++;
		}
		else
		{
			non_final[non_fin_index].final = false;

			non_final[non_fin_index].state_num = dfa[i].state_num;
			for(int j=0;j<num_alpha;++j){
				non_final[non_fin_index].transitions[j] = dfa[i].transitions[j];
			}
			non_fin_index++;
		}

	}
	assert(fin_index==num_final_states);
	return;
}


void dfs_reduce_dfa(DFA_State* dfa,int num_states,int num_alpha,char alphabets[MAX_ALPHA+1],bool reachable[])
{
	
	for (int i = 0; i < (num_states); ++i)
	{
		reachable[i] = false;
	}
	for(int i=0;i < num_states;i++){
		for (int j = 0; j < num_alpha; ++j){
			if(dfa[i].transitions[j] != i){
				reachable[dfa[i].transitions[j]] = true;
			}
		}
	}
	reachable[dfa[0].state_num] = true;	//start state is always reachable
	
	for (int i = 0; i < num_states; ++i)
	{
		if(!reachable[dfa[i].state_num])
			printf("Unreachable State:\nState %d not reachable\n",dfa[i].state_num);
	}

}


void display_dfa_entered(DFA_State* dfa,int num_states,int num_alpha,char alphabets[])
{
	printf("State Number\tSymbol Number\tTransition State\n");
	for (int i = 0; i < num_states; ++i)
	{

		printf("%d",dfa[i].state_num);
		if(dfa[i].state_num == 0)
			printf("+");
		if(dfa[i].final){
			printf("*");
		}
		for (int j = 0; j < num_alpha; ++j)
		{
			printf("\t\t%c\t\t",alphabets[j]);
			printf("%d\n",dfa[i].transitions[j]);
		}
		printf("\n");
	}
}
int main()
{

	DFA_State* dfa=NULL;
	char alphabets[MAX_ALPHA+1];
	int num_states,num_final_states,num_alpha;
	Status sc = input_dfa(&dfa,&num_alpha,&num_final_states,&num_states,alphabets);
	DFA_State final[num_final_states];
	DFA_State non_final[num_states - num_final_states];

	if(sc == SUCCESS)
	{
		reachable=(bool*)malloc(sizeof(bool)*num_states);
		dfs_reduce_dfa(dfa,num_states,num_alpha,alphabets,reachable);
		display_dfa_entered(dfa,num_states,num_alpha,alphabets);

		bool segregate[num_states][num_states];
		for (int i = 0; i < num_states; ++i)
		{
			for(int j = 0;j < num_states; ++j)
			{
				segregate[i][j] = false;
			}
		}


		for (int i = 0; i < num_states; ++i)
		{
			for(int j = 0; j < i ; j++)
			{
				assert(dfa[i].state_num == i);
				if(dfa[i].final != dfa[j].final)
				{
					segregate[i][j] = true;
				}
			}
		}

		#ifdef DEBUG	
			printf("HERE\n");
			for (int i = 0; i < num_states; ++i)
			{
				for(int j=0 ; j < i; ++j)
				{
					printf("%d ", segregate[i][j]);
					//if(segregate[i][j]!=true)
					//	printf("%d %d ",i,j);
				}
				printf("\n");
			}
		#endif
		bool repeat = true;
		while(repeat)
		{
			repeat = false;
			for (int i = 0; i < num_states; ++i)
			{
				for(int j = 0; j < i ;j++)
				{
					if(!segregate[i][j])
					{
						//check if {delta(qi,A),delta(qj,A)} is marked for any alphabet
						for (int k = 0;k < num_alpha;k++)
						{
							int state1 = dfa[i].transitions[k];
							int state2 = dfa[j].transitions[k];
							//printf("%d %d states %d\n",state1,state2,segregate[state1][state2] or segregate[state2][state1]);
							if((segregate[state2][state1] or segregate[state1][state2])){
								repeat = true;
								segregate[i][j] = true;
							}
						}
					}
				}
			}
		}

		// Combine all unmarked pair (qi,qj) and make them a single state in the reduced dfa
		#ifdef DEBUG	
			printf("HERE\n");
			for (int i = 0; i < num_states; ++i)
			{
				for(int j=0 ; j < i; ++j)
				{
					printf("%d ", segregate[i][j]);
					//if(segregate[i][j]!=true)
					//	printf("%d %d ",i,j);
				}
				printf("\n");
			}
		#endif
		
		DFA_State* pre_final_dfa=NULL;
		// MAX STATES POSSIBLE
		pre_final_dfa = (DFA_State*)malloc(sizeof(DFA_State)*num_states);

		bool info[num_states];
		for (int i = 0; i < num_states; ++i)
		{
			info[i] = false;
		}
		int pre_final_index=0;

		for (int i = 0; i < num_states; ++i)
		{
			for(int j=0; j < i; ++j)
			{
				if(!segregate[i][j])
				{
					printf("****state %d and %d represents the same state and can be used interchangeably\n",i,j);
					if(map_state.find(i) != map_state.end())
					{
						map_state[j] = map_state[i];
					}
					else if(map_state.find(j) != map_state.end())
					{
						map_state[i] = map_state[j];
					}
					else
					{
						char c_state = get_cstate();
						map_state.insert(make_pair(i,c_state));
						map_state.insert(make_pair(j,c_state));
					}
					pre_final_dfa[pre_final_index].final = dfa[i].final;
					pre_final_dfa[pre_final_index].state_num = i;
					pre_final_dfa[pre_final_index].transitions = (int*)malloc(sizeof(int)*num_alpha);
					for (int k=0;k<num_alpha;++k){
						pre_final_dfa[pre_final_index].transitions[k] = dfa[i].transitions[k];
					}
					pre_final_index++;
					info[i] = info[j] = true;
				}
			}
		}

		for (int i = 0; i < num_states; ++i)
		{
			if(info[i] == false)
			{
				printf("%d------------\n",i);
					char c_state = get_cstate();
					if(map_state.find(i) == map_state.end()){
						map_state.insert(make_pair(i,c_state));
					}
					pre_final_dfa[pre_final_index].final = dfa[i].final;
					pre_final_dfa[pre_final_index].state_num = i;
					pre_final_dfa[pre_final_index].transitions = (int*)malloc(sizeof(int)*num_alpha);
					for (int k=0;k<num_alpha;++k){
						pre_final_dfa[pre_final_index].transitions[k] = dfa[i].transitions[k];
					}
					pre_final_index++;
					info[i] = true;
			}
		}

		int size_dfa = pre_final_index;
		
		printf("\n-----------------Minimal DFA-----------------\n\n");
		bool found_start=false;
		for(auto it=map_state.begin();it!=map_state.end() and !found_start;it++)
		{
			if(it->first == 0)
			{
				printf("Start state is %c\n",it->second);
			}
		}
		display_dfa(pre_final_dfa,size_dfa,num_alpha,alphabets,true);
	
	}
	else
	{
		printf("DFA information invalid!\n");
	}
	
	return 0;
}