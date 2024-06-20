#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct list{
	struct list * n;
	char * cargo;
} list;

list * getNodeFromChar(char c) {
	list * f = malloc(sizeof(list));

	f -> cargo = malloc(sizeof(char) * 2);
	f -> cargo[0] = c;
	f -> cargo[1] = '\0';

	return f;
}

list * aToL (char * c) {
	
	list * f = getNodeFromChar(c[0]);
	list * cpy = f;

	for (int i = 1; c[i] != '\0'; i++) {
		cpy -> n = getNodeFromChar(c[i]);
		cpy = cpy -> n;
	}
	return f;
}

void printList(list * l) {
	for (list * i = l; i!= NULL; i = i-> n) {
		printf("%s",i->cargo);
	}
}

void printListAsItems(list * l) {
	for (list * i = l; i!= NULL; i = i-> n) {
		printf("%s\n",i->cargo);
	}
}

void deleteList (list * l) {
	if (l -> n != NULL) {
		deleteList(l->n);
	}
	free(l -> cargo);
	free(l);
}

int listLength(list * l) {
	int i = 0;
	for (list * it = l; l != NULL; l = l -> n) {i ++;}
	return i;
}

list * dropFirst (list * l) {
	list * s = l -> n;
	free(l -> cargo);
	free(l);
	return s;
}

void dropLast (list * l) {
	list * last = l;
	int c = 0;
	while (last-> n != NULL) {last = last -> n; c ++;}
	free(last -> cargo);
	free(last);
	last = l;
	while (c > 1) {last = last -> n; c --;}
	last -> n= NULL;
}

list * copyList (list * l ) {
	list * newL = malloc(sizeof(list));
	newL -> cargo = malloc((strlen(l->cargo) + 1) * sizeof(char));
	strcpy(newL -> cargo, l->cargo);
	
	list * returnL = newL;
	
	if (listLength(l) > 1) {
		list * it = l -> n;
		while (it != NULL) {
			newL -> n = malloc(sizeof(list));
			newL = newL -> n;
			newL -> cargo = malloc((strlen(it->cargo) + 1) * sizeof(char));
			strcpy(newL -> cargo, it->cargo);
			it = it -> n;
		}
	}

	return returnL;
}

list * copyListN(list * l, int n) {
	int count = n;
	list * newL = malloc(sizeof(list));
	newL -> cargo = malloc((strlen(l->cargo) + 1) * sizeof(char));
	strcpy(newL -> cargo, l->cargo);
	
	list * returnL = newL;
	
	if (listLength(l) > 1) {
		list * it = l -> n;
		while (it != NULL && count > 0) {
			count -- ;
			newL -> n = malloc(sizeof(list));
			newL = newL -> n;
			newL -> cargo = malloc((strlen(it->cargo) + 1) * sizeof(char));
			strcpy(newL -> cargo, it->cargo);
			it = it -> n;
		}
	} 

	newL -> n = NULL; //being explicit when you want something to be null is a good ass idea. Not doing it here shrecks the list copy severely 

	return returnL;
}

char last(char * c) {
	int i = 0;
	while (c[i] != '\0') {i ++;}
	return c[i-1];
}

char first(char * c) {
	return c[0];
}

void dropNextNode(list * l) {
	list * trash = l -> n;

	if (l -> n -> n) {
		l -> n = l -> n -> n;
	} else {
		l -> n = NULL;
	}

	free(trash ->cargo);
	free(trash);
}

int mergable(char a, char b) {
	char unmergable [] = "(){}\" ";	
	int i = 0;
	while (unmergable[i] != '\0') {
		if (a == unmergable[i]) {
			return 0;
		} else if (b == unmergable[i]) {
			return 0;
		}
		i++;
	}
	return 1;
}

void mergeListItems(list * l) {
	list * nl = l ->n;
	char * buff = malloc(sizeof(char) * (strlen(l -> cargo) + strlen(nl ->cargo) + 1 ));

	strcpy(buff, l->cargo);
	strcpy(buff + strlen(l->cargo), nl -> cargo);

	free(l -> cargo);
	free(nl -> cargo);
	l -> cargo = buff;
	l -> n = nl -> n;
	free(nl);
}

void merge(list * l) {
	list * nl = l -> n;
	int valid = mergable(last(l -> cargo),first(nl -> cargo));

	//printf("Merge on:%c.%c.%d.\n", last(l->cargo), first(nl -> cargo), valid);

	if (valid == 1) {
		mergeListItems(l);
		if (l -> n != NULL) {
			merge(l);
		} 
	} else {
		if (l -> n != NULL && l -> n -> n != NULL) {
			merge(l -> n);
		}
	}
}

int findFirstOpening(list * l) {
	list * item = l;
	int i = 0;
	do {
		if (item -> cargo[0] == '{' || item -> cargo[0] == '\"' || item -> cargo[0] == '(' ) {
			return i;		
		}
		item = item -> n;
		i++;
	} while (item != NULL);
	return -1;
}

list * getNth (list * l, int i) {
	list * it = l;
	int count = i;
	while (count > 0) {
		it = it -> n;
		count --;
	}	
	return it;
}

int getFriendBracket(list * l, char b) {
	list * it = l;
	char bType = l -> cargo[0];
	char match;
	int depth = 0;
	int count = 0;

	if (bType == '{') {
		match = '}';	
	} else if (bType == '(') {
		match = ')';	
	} else if (bType == '"') {
		match = '"';	
	}
	
	//printf("Looking for: %c.\n", match);
	if (bType == '"') {
		
		count++;
		it = it -> n;
		while (it != NULL) {
			if (it -> cargo[0] == match) {
				return count; 
			}
		count ++;
		it = it -> n;
		}

	} else {
		while (it != NULL) {
			//printf("Current thing %s\n",it -> cargo);
			//printf("Depth:%d, Count:%d\n",depth, count);
			if (it -> cargo[0] == bType) {
				depth ++;
			} else if (it -> cargo[0] == match) {
				depth --;
				if (depth == 0) {
					return count; 
				}	
			}
			count++;
			it = it -> n;
		}
	}

	return -1;
}

enum exprType {EXPR, VAL, STRING, LIST, ERROR};

typedef struct expr {
	//type 
	int exprType;
	char * vData;
	list * sData; 
	list * lData; 

	//prev, next
	struct expr * c;	
	struct expr * n;
	
} expr;

void printTree(expr *tree) {
	switch(tree -> exprType) {
		case (EXPR):
			printf("New expr\n");
			if (tree -> c) {printTree(tree -> c);}	
			printf("Done expr\n");
			if (tree -> n) {printTree(tree -> n);}	
			break;
		case (VAL):
			printf("Val: %s\n", tree->vData);
			if (tree -> n) {printTree(tree -> n);}	
			break;
		case (STRING):
			printf("String:\n");
			printList(tree->sData);
			printf("\n");
			if (tree -> n) {printTree(tree -> n);}	
			break;
		case (LIST):
			printf("List:\n");
			printList(tree->lData);
			printf("\n");
			if (tree -> n) {printTree(tree -> n);}	
			break;
		default:
			exit(1);
			printf("Broken tree node");
	}
}

void deleteTree(expr *tree) {
	switch(tree -> exprType) {
		case (EXPR):
			if (tree -> c) {deleteTree(tree -> c);}	
			if (tree -> n) {deleteTree(tree -> n);}	
			free(tree);
			break;
		case (VAL):
			if (tree -> n) {deleteTree(tree -> n);}	
			free(tree->vData);
			free(tree);
			break;
		case (STRING):
			deleteList(tree->sData);
			if (tree -> n) {deleteTree(tree -> n);}	
			free(tree);
			break;
		case (LIST):
			deleteList(tree->lData);
			if (tree -> n) {deleteTree(tree -> n);}	
			free(tree);
			break;
		default:
			printf("Broken tree node");
			exit(1);
	}
}

expr * listToTree (list * l) {
	//printf("list to tree: ");
	//printList(l);
	//printf("\n");
	//find first item ignoring spaces. Determine type, ie (, {, or ". 
	int firstRelevant = findFirstOpening(l);
	
	if (firstRelevant != -1) {
		list * opening = getNth(l,firstRelevant);	
		//printf("Found:%c\n",opening->cargo[0]);

		int fbCount = getFriendBracket(opening, opening->cargo[0]);
		list * closing = getNth(opening, fbCount); 
		
		//printf("Found count:%d\n",fbCount);
		//printf("Found:%c\n",closing->cargo[0]);
			
		list * insides = copyListN(opening, fbCount);
		//printList(insides);
		//printf("\n");

		//I guess i make a tree node.
		expr * tree = malloc(sizeof(expr));
		tree -> exprType = EXPR;

		int count = 0;
		expr * iterableTree = tree;

		//variables for inside of loop
		expr * subTree;
		list * subList;
		list * itClosing;
		int c;

		//go through the expressions arguements 
		for (list * it = opening -> n; it != closing; it = it -> n) {
			if (strcmp(" ", it ->cargo) != 0) {
				if (it -> cargo[0] == '('){
					//printf("Found (\n");
				
					c = getFriendBracket(it, it->cargo[0]);
					itClosing = getNth(it, c);
					subList = copyListN(it, c);	
					

					printList(subList);
					//Don't think this is the probelm because printing the list allready kills it
					subTree = listToTree(subList); 


					if (count == 0) { iterableTree -> c = subTree; } else {  iterableTree -> n = subTree; }
					
					iterableTree = subTree;

					if (itClosing -> n!= closing ) {it = itClosing->n;} else {break;}
					//printf("Next node being looked at: %s \n", it -> n -> cargo);

				} else if (it -> cargo[0] == '{'){
					int c2 = getFriendBracket(it, it ->cargo[0]);
					list * itClosing2 = getNth(it, c2);
					
					//constructing a list is probably not actually worth a recursive call.
					expr * listSubTree = malloc(sizeof(expr));
					listSubTree -> exprType = LIST;
					listSubTree ->lData = copyListN(it, c2);
					
					if (count == 0) { iterableTree -> c = listSubTree; } else {  iterableTree -> n = listSubTree; }
					iterableTree = listSubTree;

					//printf("Closing bracket:%s\n", itClosing2 -> n -> cargo);
					if (itClosing2 -> n != closing ) {it = itClosing2 ->n;} else {break;}

				} else if (it -> cargo[0] == '"'){
					int c3 = getFriendBracket(it, it ->cargo[0]);
					list * itClosing3 = getNth(it, c3);
					//printf("Distance to friend: %d.",c3);

					
					//constructing a list is probably not actually worth a recursive call.
					expr * stringSubTree = malloc(sizeof(expr));
					stringSubTree-> exprType = STRING;
					stringSubTree->sData = copyListN(it, c3);
					
					if (count == 0) { iterableTree -> c = stringSubTree; } else {  iterableTree -> n = stringSubTree; }
					iterableTree = stringSubTree;

					//printf("Closing bracket:%s\n", itClosing3 -> n -> cargo);
					if (itClosing3 -> n != closing ) {it = itClosing3 ->n;} else {break;}

				} else  {
					//printf("Cargo%s\n",it -> cargo);

					expr * temp = malloc(sizeof(expr));
					temp -> exprType = VAL;
					temp -> vData = malloc((strlen(it -> cargo) + 1) * sizeof(char));
					strcpy(temp->vData, it->cargo);
					if (count == 0) { iterableTree -> c = temp; } else {  iterableTree -> n = temp; }
					iterableTree = temp;
				}
				count ++;
			}
		}

		deleteList(l);
		
		return tree;

	}
	
	return NULL; //caution, this line is suspect!

}

//I want to start with this because I think it will guide me 
//in terms of how I want to handle having an envonronment and how I want to do types ,

void deleteChildren(expr * t) {
		if (t->c) {
			deleteChildren(t->c);
		}

		if (t -> n) {
			deleteChildren(t->n);
		}

		if (t->lData){ deleteList(t->lData);};
		if (t->sData){ deleteList(t->sData);};
		free(t);
}

void evaluateTree(expr * t) {
	
	//for now we are evaluating only expressions
	if (t ->exprType == EXPR) {
	//printf("Evaluating expression\n");
		//evaluate all the children 
		if (t -> c) {
			expr * child = t -> c;
			do {
				evaluateTree(child);
				child = child -> n;	
			} while (child != NULL);
		}
		
		//once done evaluating, try to combine appropriately.
		//This will also end up requring an environment to look up functions 
		//I will need some sort of a gracefull way to manage different function arguments, different number of args, etc.
		//Overall, this is kinda hard!
		int result;	
		char * resultString = malloc(sizeof(char)*22);

		//really need something more gracefull that can handle more complex functions that are not a single char in length lol
		switch (t -> c -> vData[0]) {	
			case '+':
				result = atoi(t -> c -> n -> vData) + atoi(t -> c -> n -> n -> vData);	
				if (sprintf(resultString,"%d", result) > 0) {
					t ->vData= resultString;
				} else {
					printf("Bad addition");
					exit(1);
				}
				break;
			case '-':
				result = atoi(t -> c -> n -> vData) - atoi(t -> c -> n -> n -> vData);	
				if (sprintf(resultString,"%d", result) > 0) {
					t ->vData= resultString;
				} else {
					printf("Bad subtraction");
					exit(1);
				}
				break;
			break;
			case '*':
				result = atoi(t -> c -> n -> vData) * atoi(t -> c -> n -> n -> vData);	
				if (sprintf(resultString,"%d", result) > 0) {
					t ->vData= resultString;
				} else {
					printf("Bad multiplication");
					exit(1);
				}
				break;
			case '/':
				result = atoi(t -> c -> n -> vData) / atoi(t -> c -> n -> n -> vData);	
				if (sprintf(resultString,"%d", result) > 0) {
					t ->vData= resultString;
				} else {
					printf("Bad division");
					exit(1);
				}
				break;
			break;
		}
				

		t -> exprType = VAL;


		deleteChildren(t -> c);
		t -> c = NULL;
		
			
	} else {
		//other types will need to be evaluated
		//Basically that just means that the symbol could be a variable that needs to be replaced
	}

}

typedef struct envNode {
	struct envNode * n;
	char * symbol;

	//user functtion: via expected tree and resultant tree
	expr * args; 
	expr * output;	

	//builtin functions: function via function pointers 
	expr* (* output_fun) (expr*);
} envNode;

expr * builtin_add(expr * t) {
	expr * result = malloc(sizeof(expr));
	result ->exprType = VAL;
	int count = 0;
	expr * it = t->n;
	while (it != NULL) {
		count += atoi(it->vData);
		it = it -> n;
	}
	result ->vData = malloc(sizeof(char)*20);
	sprintf(result->vData, "%d", count);	
	printf("Returning:%s.\n", result->vData);
	return result;
}


expr * builtin_sub(expr * t) {
	expr * result = malloc(sizeof(expr));
	result ->exprType = VAL;
	int count = 0;
	expr * it = t->n;
	count += atoi(it->vData);
	it = it -> n;
	while (it != NULL) {
		count -= atoi(it->vData);
		it = it -> n;
	}
	result ->vData = malloc(sizeof(char)*20);
	sprintf(result->vData, "%d", count);	
	return result;
}

expr * builtin_mult(expr * t) {
	expr * result = malloc(sizeof(expr));
	result ->exprType = VAL;
	int prod;
	expr * it = t->n;
	prod = atoi(it->vData);
	it = it -> n;
	while (it != NULL) {
		prod *= atoi(it->vData);
		it = it -> n;
	}
	result ->vData = malloc(sizeof(char)*20);
	sprintf(result->vData, "%d", prod);	
	return result;
}

expr * builtin_div(expr * t) {
	expr * result = malloc(sizeof(expr));
	result ->exprType = VAL;
	int prod;
	expr * it = t->n;
	prod = atoi(it->vData);
	it = it -> n;
	while (it != NULL) {
		if (atoi(it -> vData) == 0) {
			result -> exprType = ERROR;
			return result;
		}
		prod /= atoi(it->vData);
		it = it -> n;
	}

	result ->vData = malloc(sizeof(char)*20);
	sprintf(result->vData, "%d", prod);	
	return result;
}

expr * builtin_mod(expr * t) {
	expr * result = malloc(sizeof(expr));
	result ->exprType = VAL;
	int prod;
	expr * it = t->n;
	prod = atoi(it->vData);
	it = it -> n;
	while (it != NULL) {
		prod %= atoi(it->vData);
		it = it -> n;
	}
	result ->vData = malloc(sizeof(char)*20);
	sprintf(result->vData, "%d", prod);	
	return result;
}

//which basic list/string functions do I need to get? bit of an L that these are complicated
//get index 
//head
//tail 
//set index
//push 
//length
//concatenate
//none of these are really hard!

//once these are completed, can look at two more functions: 
//var (name) (value/list/string) --should be ok  but kinda interesting 
//fun ({args}) (expr) --suspect this one of being hard 
//eval (list) --should be ok but kinda interesting 

void add_builtin_function (envNode * origionalNode, char s [], expr * (* output_fun) (expr * t)) {
	envNode * it;
	for (it = origionalNode; it -> n != NULL; it = it -> n) {};
	
	envNode * n;
	n = malloc(sizeof(envNode));
	n -> symbol = malloc(sizeof(char)*(strlen(s)+1));
	strcpy(n->symbol, s);
	n -> output_fun = output_fun; 
	n -> n = NULL;

	it -> n = n;
}

envNode * makeEnvironment () {
	envNode * firstNode = malloc(sizeof(envNode));
	firstNode->symbol = malloc(sizeof(char)*2);
	strcpy(firstNode->symbol, "+");
	firstNode->output_fun = &builtin_add; //this seems right to me 
	
	//once the first node is created, the others can be added easily 
	add_builtin_function(firstNode,"-", &builtin_sub);
	add_builtin_function(firstNode,"*", &builtin_mult);
	add_builtin_function(firstNode,"/", &builtin_div);
	add_builtin_function(firstNode,"%", &builtin_mod);

	return firstNode;
}

int envNodeExists(envNode * n, char * s) {
	envNode * it = n;
	int count = 0;
	do {
		if (strcmp(it ->symbol, s) == 0) { return count; }
		count ++;
		it = it -> n;
	} while (it != NULL);

	return -1;
}

envNode * getNode (envNode * n, int c) {
	int count = c;
	envNode * it = n;
	
	do {
		if (count == 0) {
			return it;
		}
		count --;
		it = it -> n;
	} while (it != NULL);
	
	return n;
}

void deleteNode (envNode * n, envNode * target) {
	envNode * it = n;
	do {
		if (strcmp(it -> n ->symbol, target -> symbol) == 0) {
			
			envNode * next = it ->n -> n;
	
			if (it -> n ->output) {deleteTree(it -> n -> output);}	
			if (it -> n ->args) {deleteTree(it -> n -> args);}	
			if (it -> n ->symbol) {free(it -> n -> symbol);}	

			if (next) {it -> n = next;} else {it -> n = NULL;}
			break;
		}
	} while (it != NULL);
}

//this is basically a dummy. not terrible but needs some logic to figure args and out out. I guess that will come from the builtin function that handles assignments. Who knows.
void addNode (envNode * e, char * s, expr * args, expr * out) {
	//check if node with symbol exists 
	int count;
	if ((count = envNodeExists(e, s)) != -1) {
		deleteNode(e, getNode(e,count));
	}

	envNode *it;
	for (it = e; it -> n != NULL; it = it->n);	
	
	envNode * n = malloc(sizeof(envNode));
	it -> n = n;

	n ->symbol = malloc(sizeof(char) * (1 + strlen(s)));
	strcpy(n->symbol, s);
	n -> args = args;
	n ->output = out;
}

void printEnvironment(envNode * e) {
	envNode * it = e;
	while (it != NULL) {
		printf("Env:%s.\n",it->symbol);
		it = it -> n;
	}
}

//At the moment the goal should be to get the envionrment and basic functions there in to works properly.
//Step after that is complex functions like variable assignments, etc.
//THIS IS THE LAST THING I CODE TODAY!
void evaluateTreeWithEnvironment(envNode * e, expr * t) {
	
	//for now we are evaluating only expressions, extension to variables lists, etc. should be trivial
	if (t ->exprType == EXPR) {
	printf("Evaluating expression\n");
		
		//evaluate all the children 
		if (t -> c) {
			expr * child = t -> c;
			do {
				evaluateTreeWithEnvironment(e,child);
				child = child -> n;	
			} while (child != NULL);
		}


		int fIndex = envNodeExists(e, t -> c ->vData);


		if (fIndex >= 0) {
			
			envNode * envFunction = getNode(e,fIndex);
			expr * result;

			if (*(envFunction -> output_fun)) {
				result = envFunction -> output_fun(t->c);

			} else {
				//custom function
				//result = ...
				printf("Custom functions not yet surported\n");	
				exit(1);
			}

			//ASSUMPTION: THE RESULT DOES NOT HAVE A -> n. Hence -> n stays from origional. THIS ASSUMPTION IS GENUINELY IMPORTANT

			deleteChildren(t -> c);
			t -> c = NULL;
			if (t->vData) {free(t->vData);}
			if (t->lData) {deleteList(t->lData);}
			if (t->sData) {deleteList(t->sData);}

			if (result->exprType) { t -> exprType = result -> exprType;}
			if (result->sData) {t->sData = result->sData;}
			if (result->lData) {t->lData = result -> lData;}
			if (result->vData) {t->vData = result -> vData;}
			if (result->c) {t->c = result -> c;}

			free(result); //fairly confident this line is fine and won't kill anything since everything in result is malloced but we will see 

		} else {
			printf("Function not in environment\n");
			exit(1);
		}

	} else {
		//non expressions go here
	}

}

int main() {
	envNode * environment = makeEnvironment();
	char * doing = "(% 7 2)"; 
	//slight tree screws up if () before val 
	//completely breaks if no letter after a ))
	list * l = aToL(doing);
	merge(l);
	expr * t = listToTree(l);
	evaluateTreeWithEnvironment(environment,t);
	printTree(t);
}
