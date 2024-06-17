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

enum exprType {EXPR, VAL, STRING, LIST};

typedef struct expr {
	//type 
	int exprType;
	char * vData;
	list * sData; 
	list * lData; 

	//prev, next
	struct expr * c;	
	struct expr * n;
	
	//evaluated type
	int evaluatedType;
	char * vDataE;
	list * sDataE; 
	list * lDataE;
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


expr * listToTree (list * l) {
	printf("list to tree: ");
	printList(l);
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

		//go through the expressions arguements 
		for (list * it = opening -> n; it != closing; it = it -> n) {
			if (strcmp(" ", it ->cargo) != 0) {
				if (it -> cargo[0] == '('){
					int c = getFriendBracket(it, it->cargo[0]);
					list * itClosing = getNth(it, c);
					
					list * subList = copyListN(it, c);	
					expr * subTree = listToTree(subList);
					
					if (count == 0) { iterableTree -> c = subTree; } else {  iterableTree -> n = subTree; }
					
					printf("Finished inner tree.\n");
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

					//printf("STRING...\n");
					printList(copyListN(it, c3));
					//printf("\n\n\n\n");
					
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
	//pure void if no opening is found.
}

//I want to start with this because I think it will guide me 
//in terms of how I want to handle having an envonronment and how I want to do types ,

expr * evaluateTree() {
	//1. call evaluate tree on all child expressions
	//2. evaluate current expression once results are in
}

//typedef struct envObject {
//
//} envObject;


int main() {
	char * doing = "(a \" nice \")"; 
	//slight tree screws up if () before val 
	//completely breaks if no letter after a ))
	list * l = aToL(doing);
	merge(l);
	
	expr * t = listToTree(l);
	printTree(t);
}
