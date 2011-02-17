#include "everything.h"
#include "cexcept.h"
#include "parser.h"
#include <unistd.h>


struct exception_context the_exception_context[1];

int main(int argc, char **argv){
	exception e;
	AutoPilot *ap = NULL;
	struct tm *tm = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	UCSChar *mo_type = NULL;
	char *f_value = NULL;
	int f_flag = 0, i = 0, c, n = 0;

	while ((c = getopt(argc, argv, "f:")) != -1) {
		switch (c) {
		case 'f':
			f_flag = 1;
			f_value = optarg;
			break;
		case '?':
			if (optopt == 'f')
				fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else
			    fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
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

	Try{
		vg = createVTDGen();
		if (parseFile(vg,FALSE,f_value)){
			ap = createAutoPilot2();
			vn = getNav(vg);

			stack_node *values = NULL, *name = NULL, *sw = NULL, *granularity = NULL, *timestamp = NULL, *counters = NULL, *moid = NULL, *tmp = NULL, *type = NULL;

			name = get_text_from_xpath(vn,L"/mdc/md/neid/neun/text()");
			sw = get_text_from_xpath(vn,L"/mdc/md/neid/nesw/text()");
			type = get_text_from_xpath(vn,L"/mdc/mfh/st/text()");
			values = get_text_from_xpath(vn,L"/mdc/md/mi/mv/r/text()");
			wprintf(L"Processing %s with SW v%s \n",name->data,sw->data);
			selectXPath(ap,L"/mdc/md/mi");
			bind(ap,vn);
			while((i=evalXPath(ap))!=-1){
				counters = get_text_from_xpath(vn,L"mt/text()");
				moid = get_text_from_xpath(vn,L"mv/moid/text()");
				timestamp = get_text_from_xpath(vn,L"mts/text()");
				mo_type = get_moid_type(moid->data);

				granularity = get_text_from_xpath(vn,L"gp/text()");
				wprintf(L"mo type is %s \n",mo_type);

				//wprintf(L"filename is %s \n",file_name);


				//tm = get_datetime_from_string(timestamp->data);
				//tm->tm_sec -= (int)wcstol(granularity->data, 0, 10);
				//mktime(tm); //normalize

				//printf( "%s\n", asctime( tm));
				print_stdout(&counters,&moid,&values);

				free(tm);free(timestamp);free(granularity);
				free(counters); free(moid);
			}

			free(name); free(sw); free(values);
			freeAutoPilot(ap);
			free(vn->XMLDoc);
		}
		else {
			free(vg->XMLDoc);
		}
	}
	Catch (e) {// handle various types of exceptions here
		fwprintf(stderr,L"An exception occurred => %s \n %s\n",e.msg,e.sub_msg);
		freeVTDGen(vg);
	}
	free(f_value);
	freeVTDNav(vn);
	freeVTDGen(vg);
	return 0;
}
UCSChar *make_file_name(const UCSChar *pm_type, const UCSChar *mo_type, const UCSChar *timestamp, const UCSChar *sw, const UCSChar *name, const UCSChar *granularity) {
	UCSChar *file_name = (UCSChar *)malloc(200 * sizeof(UCSChar));
}

/* convert a char string to a w_char string */
UCSChar *char_to_wchar(const char **source) {
	mbstate_t state = NULL;
	int n = strlen(*source);
	UCSChar *string = (UCSChar *)malloc((n+1) * sizeof(UCSChar));
	mbsrtowcs(string,source,n,&state);
	string[n] = '\0';
	free(state);
	free(n);
	return string;
}

int file_exists(const char *filename) {
	return !access(filename, F_OK);
}

/* given a moid, return the type of moid that the string contains (determined by splitting the string up) */
UCSChar *get_moid_type(const UCSChar *moid) {
	UCSChar *index = NULL, *last = NULL;
	UCSChar *temp = (UCSChar *)w_substr(moid,0,wcslen(moid));

	index = wcstok(temp,L",");
	while(index) {
		last = index;
		index = wcstok(NULL,L",");
	}
	index = wcstok(last,L"=");
	return index;
}

/* print values to stdout */
void print_stdout(stack_node **counters, stack_node **moid, stack_node **values) {
	int count=0, i;
	UCSChar *counter = NULL, *value = NULL, *mo = NULL;

	//print the header line
	wprintf(L"moid");
	while (counter = unshift_from(counters)) {
		wprintf(L";%s", counter);
		count++;
	}
	wprintf(L"\n");
	//print the values for each mo (managed object)
	while (mo = unshift_from(moid)) {
		wprintf(L"%s",mo);
		for (i=0;i < count; i++) {
			value = unshift_from(values);
			wprintf(L";%s", value);
		}
		wprintf(L"\n");
	}
	wprintf(L"\n");
	free(counter); free(value); free(mo);
}


/* print counter and values to CSV file */
void print_to_csv(const UCSChar *file_name,stack_node **counters, stack_node **moid, stack_node **values) {
	FILE *out_csv;
	int count=0, i;
	UCSChar *counter = NULL, *value = NULL, *mo = NULL;
	wprintf(L"FILENAME is %s \n",file_name);
	if ((out_csv = _wfopen(file_name,L"w")) == NULL) {
		fwprintf(stderr,L"File %s could not be opened ! \n",file_name);
	}
	else {
		//print the header line
		fwprintf(out_csv,L"moid");
		while (counter = unshift_from(counters)) {
			fwprintf(out_csv,L";%s", counter);
			count++;
		}
		fwprintf(out_csv,L"\n");
		//print the values for each mo (managed object)
		while (mo = unshift_from(moid)) {
			fwprintf(out_csv,L"%s",mo);
			for (i=0;i < count; i++) {
				value = unshift_from(values);
				fwprintf(out_csv,L";%s", value);
			}
			fwprintf(out_csv,L"\n");
		}
		fwprintf(out_csv,L"\n");
		free(counter); free(value); free(mo);
	}
	fclose(out_csv);
}

/* retrieve a wide character substring from a source string, given a starting position and length of characters to copy */
UCSChar *w_substr(const UCSChar *source,size_t start, size_t len) {
	UCSChar *result = (UCSChar *) malloc((len + 1)*sizeof(UCSChar));
	wmemcpy(result,source+start,len);
	result[len] = '\0';
	return result;
}

/* return  a struct tm time structure, given an input string in the format YYYYMMDDhhmmss */
struct tm *get_datetime_from_string(const UCSChar *string) {
	struct tm *tm = (struct tm *)malloc(sizeof(struct tm));
	tm->tm_year = (int)wcstol(w_substr(string,0,4), 0, 10) - 1900;
	tm->tm_mon = (int)wcstol(w_substr(string,4,2), 0, 10) - 1;
	tm->tm_mday = (int)wcstol(w_substr(string,6,2), 0, 10);
	tm->tm_hour = (int)wcstol(w_substr(string,8,2), 0, 10);
	tm->tm_min = (int)wcstol(w_substr(string,10,2), 0, 10);
	tm->tm_sec = (int)wcstol(w_substr(string,12,2), 0, 10);
	tm->tm_isdst = 0;
	mktime(tm);      // normalize time
	return tm;
}

/* given an xpath, return the text node value */
stack_node *get_text_from_xpath(const VTDNav *v_nav, const UCSChar *x_path) {
	AutoPilot *ap = NULL;
	UCSChar *result = NULL;
	stack_node *stack_ptr = NULL; //points to the stack top
	int i;

	wprintf(L"Getting %s \n",x_path);
	ap = createAutoPilot2();
	selectXPath(ap,x_path);
	bind(ap,v_nav);
	push(v_nav);

	while((i=evalXPath(ap))!=-1){
		result = toString(v_nav,i);
		push_to(&stack_ptr,result);
	}
	resetXPath(ap);
	pop(v_nav);
	if (result == NULL) {
		fwprintf(stderr,L"Warning: result of xpath expression \"%s\" is NULL! \n",x_path);
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
		wprintf(L"Could not push %s onto stack because no memory is available.\n",value);
	}
}

void print_stack(stack_node *stack) {
	if (stack == NULL) {
		printf("The stack is empty.\n\n");
	}
	else {
		printf("The stack is:\n");
		while (stack != NULL) {
			wprintf(L"%s -> ", stack->data);
			stack = stack->next;
		}
		printf("NULL\n\n");
	}
}

