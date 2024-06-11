#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char input [2000];
typedef struct word {
	char * text;
	struct word * next;
} word;

word * makeWord(char input []) {
	word * w = malloc(sizeof(word));
	char * buff = malloc(sizeof(char)*(strlen(input) + 1));
	strcpy(buff,input); //copies the null terminator
	w-> text = buff;
	return w;
}


//realized that getting rid of the first item on a zero or one element list is meaningless, hence null is returned.
char * withoutFirst(char * buff) {
	int l = strlen(buff);
	if (l > 1) {
		char * new = malloc(sizeof(char)*l); //new is one too small 
		strcpy(new,buff+1); //strcpy does the null terminator //but I take one away from old
		return new;
	} else {
		return NULL;
	}
}

//this is quite prone to bugs. There is an awkward thing going on between the last string character and the last character
char * withoutLast(char * buff) {
	int l = strlen(buff);
	if (l > 1) {
		char * new = malloc(sizeof(char)*l); //one too few, recall size does not include /0
		strncpy(new,buff,l-1); //copy one character less than in the origional string, note that we are talking characters, not including \0
		new[l-1] = '\0'; //manually add \0. index must be one less than sizeof the array.
		return new;
	} else {
		return NULL;
	}
}

char * firstN(char * buff, int i) {
	int l = strlen(buff);
	if (i <= l && i > 0) {
		char * new = malloc(sizeof(char)*(i+1)); //note that i characters can be copied
		strncpy(new,buff,i); //this is a number of characters. note that this is the input. ie i chars are copied, as wished.
		new[i] = '\0'; //this is an index. previous line never touches index i. ends at i-1
		return new;
	} else {
		return NULL;
	}
}

char * afterN(char * buff, int i) {
	int l = strlen(buff);
	if (i < l && i>=0) {
		char * new = malloc(sizeof(char)*l-i+1);
		strcpy(new,buff+i);
		return new;
	} else {
		return NULL;
	}
}

//int countSpace(char * a) {
//	int i = 0;
//	int j = 0;
//	while (a[i] != '\0') {if (a[i] == ' ') {j++;}  i++;}
//	return j;
//}
//
//int indexOfFirstSpace(char * a) {
//	int i = 0;
//	while (a[i] != '\0') {if (a[i] == ' ') {return i;}  i++;}
//	return 0;
//}

int countChar(char * a, char c) {
	int i = 0;
	int j = 0;
	while (a[i] != '\0') {if (a[i] == c) {j++;}  i++;}
	return j;
}

int indexOfFirstChar(char * a, char c) {
	int i = 0;
	while (a[i] != '\0') {if (a[i] == c) {return i;}  i++;}
	return 0;
}

void splitWord(word * w, char target) {
	//1. Are there spaces in the word. 
	int c = countChar(w->text, target);
	int l = strlen(w->text);

	if (c == l) {
		printf("Bad node found\n");
		free(w -> text);
		char * buff = malloc(sizeof(char));
		strcpy(buff,""); 
		printf("Bad node text set to empty string\n");
		w -> text = buff;	
	} else {

		if (c > 0) {

			int i = indexOfFirstChar(w->text,target);

			if (i == 0) {
				char * temp = withoutFirst(w->text);
				free(w->text);
				w->text = temp;
				splitWord(w,target);	
			} else if (i == l-1) {
				char * temp = withoutLast(w->text);
				free(w->text);
				w->text = temp;
			} else {
				//a new word is reqFirstuired 
				word * wn = malloc(sizeof(word));
				wn -> next = w-> next;
				w-> next = wn;

				//next these thing must be filled in 
				char * firstPart = firstN(w->text, i); //this will miss the space 
				char * lastPart = afterN(w->text, i+1); //this will also miss the space 
				
				free(w->text);
				w -> text = firstPart;
				wn ->text = lastPart;
				splitWord(w->next,target);
			}

			//it is possible that the critical error in the previous cases was that the recursive calls are interfering with each other 
			//please be very carefull about the use of recursion in this program. Pherhaps only call the function on next if it has been cleared of spaced
		} else {
			if (w -> next) {
				splitWord(w->next,target);
			}
		}

	}
}

void seperateSpecialChar(word *w, char c) {
	//hoenestly the use of without last, without first, and fist n/after n is very similar
	//case (ab
	//case ab(
	//case a(b case (
	int count = countChar(w->text, c);

	if (count == 0) {

	} else {
		int len = strlen(w->text);
		int i = indexOfFirstChar(w->text,c);
			
		if (i == 0 && len == 1) {
			//case {
		} else if (i == 0) {
			//case {a
			char * newBuff = withoutFirst(w->text);
			char * oldBuff = malloc(2*sizeof(char));

			oldBuff[0] = c;
			oldBuff[1] = '\0';

			word * nw = malloc(sizeof(word));
			free(w->text);
			w -> text = oldBuff;
			nw-> text = newBuff;
			
			nw -> next = w-> next;
			w-> next = nw;

		} else if (i == len -1) {
			//case a{
			char * oldBuff = withoutLast(w->text);
			char * newBuff = malloc(2*sizeof(char));
		
			newBuff[0] = c;
			newBuff[1] = '\0';
			
			word * nw = malloc(sizeof(word));
			free(w->text);
			w -> text = oldBuff;
			nw -> text = newBuff;
			
			nw -> next = w->next;
			w -> next = nw;

		
		} else {
			//case a{a -- carefull with this one
			char * oldBuff = firstN(w->text, i);
			char * newBuff = afterN(w-> text, i); //going to do a bingus thing

			word * nw = malloc(sizeof(word));

			free(w->text);
			w -> text = oldBuff;
			nw -> text = newBuff;
			
			nw -> next = w->next;
			w -> next = nw;
			//why the fear of this being a bingus case? This doesn't make a seperate word for a {, instead it turns a a{b into a [a,{b] and lets the {a case take care of the new word.
		} 


	}


	if (w-> next != NULL) {seperateSpecialChar(w->next, c);}
}


void killBadNodes(word * w, word * n) {
	if (strcmp(n->text,"") == 0) {
		if (n -> next != NULL) {
			w -> next = n -> next;
			free(n->text);
			free(n);
			killBadNodes(w,w->next);
		} else {
			w -> next = NULL;
			free(n->text);
			free(n);
		}
	} else if (n->next != NULL){
		killBadNodes(n,n->next);
	}
}

void printWord(word * w) {
	int i = 0;
	for (word * t = w; t != NULL; t=t->next){
		printf("%s.", t->text);
	}
	printf("\n");
}

void initializeExpression(word * w){
	splitWord(w,' ');
	killBadNodes(w, w->next);
	seperateSpecialChar(w, '{'); //I think the plan is to call {} a list and () an expression. Then allow {} to be evaluated using (eval {})
	seperateSpecialChar(w, '{'); 
	seperateSpecialChar(w, ')'); 
	seperateSpecialChar(w, ')'); 
	seperateSpecialChar(w, '"');  //right now strings collapse whitespace. Boo-Hoo.
	seperateSpecialChar(w, '"'); 
	printWord(w);
}

//I guess tree stuff goes down here 

void assembleTree(word * w) {
	//to make this easier: assume all expressions are surrounded by () for normal {} for list and "" for string 
	
}

int main() {
	//there are two ways to solve the space probelm. Way 1: encode it away. Way 2: Rip everything appart.
	char input [] = "{ }{a{{{}} } }";//"{ {a 'this shit should stay together' {b  c  {{{{ ";
	word * w = makeWord(input);
	initializeExpression(w);
}

