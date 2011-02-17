/* 
 * Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include "nodeRecorder.h"
#define BUF_SZ_EXPO 7
NodeRecorder* createNodeRecorder2(){
	exception e;
	NodeRecorder *nr = (NodeRecorder*) malloc(sizeof(NodeRecorder));
	if (nr==NULL) {
		throwException2(out_of_mem,
			"NodeRecorder allocation failed ");
		return NULL;
	}
	nr->vn = NULL;
	Try{
		nr->fib = createFastIntBuffer2(BUF_SZ_EXPO);
	}Catch(e){
		free(nr);
		throwException2(out_of_mem,
			"NodeRecorder allocation failed ");
		return NULL;
	}

	nr->count = nr->size = nr->position = 0;
	return nr;
}

NodeRecorder* createNodeRecorder(VTDNav *vn){
	exception e;
	NodeRecorder *nr;
	if (vn==NULL){
		throwException2(invalid_argument,
			"NodeRecorder allocation failed ");
		return NULL;
	}
	nr = (NodeRecorder*) malloc(sizeof(NodeRecorder));
	if (nr==NULL) {
		throwException2(out_of_mem,
			"NodeRecorder allocation failed ");
		return NULL;
	}
	nr->vn = vn;
	Try{
		nr->fib = createFastIntBuffer2(BUF_SZ_EXPO);
	}Catch(e){
		free(nr);
		throwException2(out_of_mem,
			"NodeRecorder allocation failed ");
		return NULL;
	}

	nr->count = nr->size = nr->position = 0;
	return nr;
}

void freeNodeRecorder(NodeRecorder *nr){
	if (nr!=NULL){
		freeFastIntBuffer(nr->fib);
	}
	free(nr);
}


void bind4NodeRecorder(NodeRecorder *nr, VTDNav *vn){
	if (vn==NULL){
		throwException2(invalid_argument,
			"vn can't be NULL");
	}
	nr->vn = vn;
}

void record(NodeRecorder *nr){
            //add the context and
            int i,k;
            switch (nr->vn->context[0])
            {
                case -1:
                    appendInt(nr->fib,(int)(0xff | 0x80000000));
                    nr->size++;
                    nr->position++;
                    nr->count++;
                    break;
                case 0:
                    if (nr->vn->atTerminal == FALSE)
                    {
                        appendInt(nr->fib,0);
                        nr->count++;
                    }
                    else
                    {
                       
                        appendInt(nr->fib,(int)0x80000000);
                        
                        nr->count += 2;
                    }
                    nr->size++;
                    nr->position++;
                    if (nr->vn->atTerminal == TRUE)
                        appendInt(nr->fib,nr->vn->LN);
                    break;
                case 1:
                    if (nr->vn->atTerminal == FALSE)
                    {
                        appendInt(nr->fib,1);
                        appendInt(nr->fib,nr->vn->context[1]);
                        appendInt(nr->fib,nr->vn->l1index);
                        nr->size++;
                        nr->position++;
                        nr->count += 3;
                    }
                    else
                    {                       
                        appendInt(nr->fib,(int)(0x80000001));
                        appendInt(nr->fib,nr->vn->context[1]);
                        appendInt(nr->fib,nr->vn->l1index);
                        appendInt(nr->fib,nr->vn->LN);
                        nr->size++;
                        nr->position++;
                        nr->count += 4;
                    }
                    break;
                case 2:
                    if (nr->vn->atTerminal == FALSE)
                    {
                        appendInt(nr->fib,2);
                        nr->count += 7;
                    }
                    else
                    {
                        appendInt(nr->fib,(int)0x80000002);                        
                        nr->count += 8;
                    }
                    appendInt(nr->fib,nr->vn->context[1]);
                    appendInt(nr->fib,nr->vn->context[2]);
                    appendInt(nr->fib,nr->vn->l1index);
                    appendInt(nr->fib,nr->vn->l2lower);
                    appendInt(nr->fib,nr->vn->l2upper);
                    appendInt(nr->fib,nr->vn->l2index);
                    nr->size++;
                    nr->position++;

                    if (nr->vn->atTerminal == TRUE)
                        appendInt(nr->fib,nr->vn->LN);

                    break;
                case 3:
                    if (nr->vn->atTerminal == FALSE)
                    {
                        appendInt(nr->fib,3);
                        nr->count += 11;
                    }
                    else
                    {
                        appendInt(nr->fib,(int)(0x80000003));
                        nr->count += 12;
                    }
                    appendInt(nr->fib,nr->vn->context[1]);
                    appendInt(nr->fib,nr->vn->context[2]);
                    appendInt(nr->fib,nr->vn->context[3]);
                    appendInt(nr->fib,nr->vn->l1index);
                    appendInt(nr->fib,nr->vn->l2lower);
                    appendInt(nr->fib,nr->vn->l2upper);
                    appendInt(nr->fib,nr->vn->l2index);
                    appendInt(nr->fib,nr->vn->l3lower);
                    appendInt(nr->fib,nr->vn->l3upper);
                    appendInt(nr->fib,nr->vn->l3index);
                    nr->size++;
                    nr->position++;

                    if (nr->vn->atTerminal == TRUE)
                        appendInt(nr->fib,nr->vn->LN);

                    break;
                default:
                    if (nr->vn->atTerminal == FALSE)
                    {
                        i = nr->vn->context[0];
                        appendInt(nr->fib,i);
                        nr->count += i + 8;
                    }
                    else
                    {
                        i = nr->vn->context[0];
                        appendInt(nr->fib,(int)(i | 0x80000000));
                        nr->count += i + 9;
                    }
                    for (k = 1; k <= i; k++)
                    {
                        appendInt(nr->fib,nr->vn->context[k]);
                    }
                    appendInt(nr->fib,nr->vn->l1index);
                    appendInt(nr->fib,nr->vn->l2lower);
                    appendInt(nr->fib,nr->vn->l2upper);
                    appendInt(nr->fib,nr->vn->l2index);
                    appendInt(nr->fib,nr->vn->l3lower);
                    appendInt(nr->fib,nr->vn->l3upper);
                    appendInt(nr->fib,nr->vn->l3index);
                    nr->size++;
                    nr->position++;

                    if (nr->vn->atTerminal)
                        appendInt(nr->fib,nr->vn->LN);

                    break;
            }
}

void resetPointer(NodeRecorder *nr){
           nr->position = 0;
           nr->count = 0;
}

void clearNodeRecorder(NodeRecorder *nr){
	nr->count = nr->size = nr->position = 0;
	clearFastIntBuffer(nr->fib);
}

int iterateNodeRecorder(NodeRecorder *nr){
	int j, i;
	Boolean b;
	if (nr->count < nr->fib->size)
	{
		i = intAt(nr->fib,nr->count);
		b = (i >= 0);
		if (b == FALSE)
		{
			i = i & 0x7fffffff;
		}
		switch (i)
		{
		case 0xff:
			nr->vn->context[0] = -1;
			nr->vn->atTerminal = FALSE;
			nr->count++;
			break;

		case 0:
			nr->vn->context[0] = 0;
			if (b == FALSE)
			{
				nr->vn->atTerminal = TRUE;
				nr->vn->LN = intAt(nr->fib,nr->count + 1);
				nr->count += 2;
			}
			else
			{
				nr->vn->atTerminal = FALSE;
				nr->count++;
			}

			break;

		case 1:
			nr->vn->context[0] = 1;
			nr->vn->context[1] = intAt(nr->fib,nr->count + 1);
			nr->vn->l1index = intAt(nr->fib,nr->count + 2);
			if (b == FALSE)
			{
				nr->vn->atTerminal = TRUE;
				nr->vn->LN = intAt(nr->fib,nr->count + 3);
				nr->count += 4;
			}
			else
			{
				nr->vn->atTerminal = FALSE;
				nr->count += 3;
			}

			break;

		case 2:
			nr->vn->context[0] = 2;
			nr->vn->context[1] = intAt(nr->fib,nr->count + 1);
			nr->vn->context[2] = intAt(nr->fib,nr->count + 2);
			nr->vn->l1index = intAt(nr->fib,nr->count + 3);
			nr->vn->l2lower = intAt(nr->fib,nr->count + 4);
			nr->vn->l2upper = intAt(nr->fib,nr->count + 5);
			nr->vn->l2index = intAt(nr->fib,nr->count + 6);
			if (b == FALSE)
			{
				nr->vn->atTerminal = TRUE;
				nr->vn->LN = intAt(nr->fib,nr->count + 7);
				nr->count += 8;
			}
			else
			{
				nr->vn->atTerminal = FALSE;
				nr->count += 7;
			}

			break;

		case 3:
			nr->vn->context[0] = 3;
			nr->vn->context[1] = intAt(nr->fib,nr->count + 1);
			nr->vn->context[2] = intAt(nr->fib,nr->count + 2);
			nr->vn->context[3] = intAt(nr->fib,nr->count + 3);
			nr->vn->l1index = intAt(nr->fib,nr->count + 4);
			nr->vn->l2lower = intAt(nr->fib,nr->count + 5);
			nr->vn->l2upper = intAt(nr->fib,nr->count + 6);
			nr->vn->l2index = intAt(nr->fib,nr->count + 7);
			nr->vn->l3lower = intAt(nr->fib,nr->count + 8);
			nr->vn->l3upper = intAt(nr->fib,nr->count + 9);
			nr->vn->l3index = intAt(nr->fib,nr->count + 10);
			if (b == FALSE)
			{
				nr->vn->atTerminal = TRUE;
				nr->vn->LN = intAt(nr->fib,nr->count + 11);
				nr->count += 12;
			}
			else
			{
				nr->vn->atTerminal = FALSE;
				nr->count += 11;
			}

			break;

		default:
			nr->vn->context[0] = i;
			for (j = 1; j < i; j++)
			{
				nr->vn->context[j] = intAt(nr->fib,nr->count + j);
			}
			nr->vn->l1index = intAt(nr->fib,nr->count + i);
			nr->vn->l2lower = intAt(nr->fib,nr->count + i + 1);
			nr->vn->l2upper = intAt(nr->fib,nr->count + i + 2);
			nr->vn->l2index = intAt(nr->fib,nr->count + i + 3);
			nr->vn->l3lower = intAt(nr->fib,nr->count + i + 4);
			nr->vn->l3upper = intAt(nr->fib,nr->count + i + 5);
			nr->vn->l3index = intAt(nr->fib,nr->count + i + 6);
			if (b == FALSE)
			{
				nr->vn->atTerminal = TRUE;
				nr->vn->LN = intAt(nr->fib,nr->count + 11);
				nr->count += i + 8;
			}
			else
			{
				nr->vn->atTerminal = FALSE;
				nr->count += i + 7;
			}
			break;
		}
		nr->position++;
		return getCurrentIndex(nr->vn);
	}
	return -1;
}