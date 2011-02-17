#include "everything.h"
#include "cexcept.h"
#include "parser.h"
#include <unistd.h>


struct exception_context the_exception_context[1];

int main(int argc, char **argv){
	exception e;
	AutoPilot *ap = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	stack_node *values = NULL;
	char *f_value = NULL;
	int f_flag = 0, p_flag = 0, c, index;

	while ((c = getopt(argc, argv, "f:p:")) != -1) {
		switch (c) {
		case 'f':
			f_flag = 1;
			f_value = optarg;
			break;
		case 'p':
			p_flag = 1;
			break;
		case '?':
			if ((optopt == 'f') || (optopt == 'p')) {
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			}
			else
			    fprintf (stderr,"Unknown option character `\\x%c'.\n",optopt);
			return 1;
		}

	}
	if (f_flag == 0) {
		fprintf(stderr, "Please specify the xml file to be parsed with the option -f.\n");
		return 1;
	}
	if (file_exists(f_value) == FALSE) {
		fprintf(stderr, "The specified file %s does not exist.\n",f_value);
		return 1;
	}
	if (p_flag == 0) {
			fprintf(stderr, "Please specify the XML Xpath to be evaluated with the option -p.\n");
			return 1;
	}

	Try{
		vg = createVTDGen();
		if (parseFile(vg,FALSE,f_value)){
			ap = createAutoPilot2();
			vn = getNav(vg);

			for (index = optind-1; index < argc; index++) {
				int n = strlen(argv[index]);
				UCSChar *var = (UCSChar *)malloc((n+1)*sizeof(UCSChar));
				int length = mbstowcs(var,(const*)argv[index], n);
				var[length] = L'\0';
				values = get_text_from_xpath(vn,var);
				if (values != NULL) {
					print_stdout(&values);
				}
			}


			free(vn->XMLDoc);
		}
		else {
			free(vg->XMLDoc);
		}
	}
	Catch (e) {// handle various types of exceptions here
		fwprintf(stderr,L"An exception occurred => %ls \n %ls\n",e.msg,e.sub_msg);
	}
	free(f_value);
	freeAutoPilot(ap);
	freeVTDNav(vn);
	freeVTDGen(vg);
	free(values);
	return 0;
}

int file_exists(const char *filename) {
	return !access(filename, F_OK);
}

/* print values to stdout */
void print_stdout(stack_node **item) {
	int count=0;
	UCSChar *value = NULL;

	while (value = unshift_from(item)) {
		wprintf(L"%ls\n", value);
		count++;
	}
	free(value);
}

/* retrieve a wide character substring from a source string, given a starting position and length of characters to copy */
UCSChar *w_substr(const UCSChar *source,size_t start, size_t len) {
	UCSChar *result = (UCSChar *) malloc((len + 1)*sizeof(UCSChar));
	wmemcpy(result,source+start,len);
	result[len] = '\0';
	return result;
}

/* given an xpath, return the text node value */
stack_node *get_text_from_xpath(VTDNav *v_nav, UCSChar *x_path) {
	AutoPilot *ap = NULL;
	UCSChar *result = NULL;
	UCSChar *result2 = NULL;
	stack_node *stack_ptr = NULL; //points to the stack top
	exception ex;
	int i;

	//wprintf(L"xpath:%s\n",x_path);
	ap = createAutoPilot2();

	Try {
		selectXPath(ap,x_path);
	}
	Catch(ex) {
		fwprintf(stderr,L"Warning: A problem occured while selecting xpath \"%ls\" \n",x_path);
	}
	bind(ap,v_nav);
	push(v_nav);
	while((i=evalXPath(ap))!=-1){
		tokenType type = getTokenType(v_nav,i);
		if (type == TOKEN_ATTR_NAME) {
			result = toString(v_nav,i+1);
		}
		else {
			result = toString(v_nav,i);
		}
		//result = toString(v_nav,i);
		push_to(&stack_ptr,result);
	}
	resetXPath(ap);
	pop(v_nav);


	if (result == NULL) {
		fwprintf(stderr,L"Warning: Result of xpath expression \"%ls\" is NULL! \n",x_path);
	}
	else {
		freeAutoPilot(ap);
	}
	//free(result);
	return stack_ptr;
}

int is_empty(stack_node **top) {
	return top == NULL;
}

UCSChar *unshift_from(stack_node **top) {
	stack_node *temp = NULL, *previous = NULL;
	UCSChar *value = NULL;
	if (!is_empty(*top)) {
		temp = (*top)->end;
		value = temp->data;
		previous = temp->previous;
		(*top)->end = previous;

		if (previous != NULL) {
			previous->next = NULL;
		}
		else {
			(*top) = NULL;
		}
		free(temp);
	}
	return value;
}

/* pop a node value from the top of the stack */
UCSChar *pop_from(stack_node **top) {
	stack_node *temp = NULL;	//used to free memory not needed anymore
	UCSChar *value = NULL;
	if (!is_empty(*top)) {
		temp = *top;
		value = (*top)->data;
		*top = (*top)->next;
		free(temp);
	}
	return value;
}

void push_to(stack_node **top,const UCSChar *value) {
	stack_node *new = NULL;
	stack_node *tmp = NULL;
	new = malloc(sizeof(stack_node));

	if (new != NULL) {
		new->previous = NULL;new->next = NULL;new->end = NULL;  //initialize
		//manage end, previous pointers
		if (is_empty(*top)) {
			new->end = new;
			new->previous = NULL;
		}
		else {
			new->end = (*top)->end;
			(*top)->previous = new;
		}

		new->data = value;
		new->next = *top;
		*top = new;

	}
	else {
		wprintf(L"Could not push %ls onto stack because no memory is available.\n",value);
	}
}

void print_stack(stack_node *stack) {
	if (stack == NULL) {
		printf("The stack is empty.\n\n");
	}
	else {
		printf("The stack is:\n");
		while (stack != NULL) {
			wprintf(L"%ls -> ", stack->data);
			stack = stack->next;
		}
		printf("NULL\n\n");
	}
}

