#include <stdio.h>
static char stack [2000];
static int maxIndex;

static int addToStack(char c) {
	maxIndex += 1;
	stack[maxIndex] = c;
	return maxIndex;
}

static char removeFromStack() {
	maxIndex -= 1;
	return stack[maxIndex];
}

enum stackResult {ERROR, BRACKET_ADDED, BRACKET_REMOVED, NO_CHANGE };

char getTop() {
	return stack[maxIndex];
}

int smartAdd(char c) {
	if ((c == '(') || c == '{') {
		addToStack(c);
		return BRACKET_ADDED;
	}  else if (c == ')') {
		if (getTop() == '(') { 
			removeFromStack(); 
			return BRACKET_REMOVED;
		} else {
			return ERROR;
		}
	} else if (c == '}') {
		if (getTop() == '(') { 
			removeFromStack(); 
			return BRACKET_REMOVED;
		} else {
			return ERROR;
		}
	} else if (c == '"') {
		if (getTop() == '"') { 
			removeFromStack(); 
			return BRACKET_REMOVED;
		} else {
			addToStack(c);
			return BRACKET_ADDED;
		}
	} else {
		return NO_CHANGE;
	}

}

//this is relatively close to what I want.
//can add an item via smart for every char and monitor the response for the change 
//depending on the response I know what to do
//worst case on change you have to hit getTop to see what the current state is and how to make the next item
//I may just require that the start and end of what the user types has to be (), ie have to be explicit about lines being expressions

int main() {

}
