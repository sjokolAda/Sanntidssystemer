#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"
list_t list_create()
{
//	new_list = (list_t)malloc(sizeof(list_t));
	list_t new_list = malloc(sizeof(struct list));	
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->length = 0;
	return new_list;

}

void list_delete(list_t list)
{
	
	struct node *temp;

	
	while (list->length > 1){
		temp = list->tail->prev;
		free(list->tail);
		temp->next=NULL;
		list->tail = temp;
		list->length--;
	}

	if (list->length == 1){
		free(list->tail);

		list->head = NULL;
		list->tail = NULL;
		list->length = 0;	
	}
	
	free(list);
}

void list_insert(list_t list, int index, int data_in)
{
	struct node *new_node = malloc(sizeof(struct node));
	//(*new_node).data = data_in;
	new_node -> data = data_in;	
	new_node -> next = NULL;
	new_node -> prev = NULL;	
	
	//test for errors
	if(list->head==NULL || index <0 || index > list->length +1){
		printf("invalid index");
	}
	//if empty list:
	if(!list->head){
		list->head = new_node;
		
		list->tail=list->head;
		list->length = 1;
	}
	
	//if insterting first element	
	if (index == 0){
	new_node -> next = list->head;
	list->head->prev = new_node;
	list->head = new_node;
	list->length++;	
	}

	// if insert at last element
	if (index == list->length+1){
		list->tail->next = new_node;
		new_node->prev = list->tail;
		list->tail = new_node;
		list->length++;
	}
	
	// insert at index
	struct node *temp;	
	temp = list->head;
	while(index > 0 ){
		temp = temp -> next;
		if (temp == NULL){
			printf("sometin bad lol");
		}
		index--;
	}
	
	temp->prev->next = new_node;
	new_node->prev = temp->prev;
	temp -> prev = new_node;
	new_node->next = temp;
	list->length++;
	

}

void list_append(list_t list, int data_in)
{
	struct node *new_node = malloc(sizeof(struct node));
	new_node -> data = data_in;
	new_node -> next = NULL;
	new_node -> prev = NULL;
	if(list->length == 0){
		list -> head = new_node;
		list -> tail = new_node;
	}
	else
	{
		list -> tail -> next = new_node;
		new_node -> next = NULL;
		new_node -> prev = list->tail;
		list -> tail = new_node;	
	}
	list->length++;
}

void list_print(list_t list)
{

	struct node *temp;
	temp = list->head;
	while(temp != NULL){
		printf("%i ",temp->data);
		temp = temp->next;
	}
	printf("\n");
			
}

long list_sum(list_t list)
{
	long int sum = 0; 
	if (list->length ==1){
		sum = list->head->data;
		return sum;
	}

	
	struct node *temp;
	temp=list->head;
	while(temp->next != NULL){
		sum = sum + temp->data;
		temp = temp->next;
		if(temp->next == NULL){
			sum = sum + temp->data;
		}
	}
	 
	return sum;
}

int list_get(list_t list, int index)
{	
	if ((index > list->length) || (index < 0)){

		return 0;
	}
	struct node *temp;
	temp = list->head;
	int i;
	for (i=1;i<=index;i++){
		temp = temp->next;
	}
	return temp->data;
}

int list_extract(list_t list, int index)
	
{
	struct node *delete_node;
	int index_data = 0;
	if ((index > list->length) || (index < 0)){

		return 0;
	}

	if (list->length == 1){
		index_data = list->head->data;
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->length--;
		return index_data;
	}
	
	if (list->length == index){
		index_data = list->tail->data;
		
		delete_node = list->tail;
		list->tail = delete_node->prev;
		list->tail->next = NULL;
		free(delete_node);
		list->length--;
		return index_data;
				
	}

	if (index == 0){
		
		delete_node = list->head;
		index_data = delete_node->data;
		list->head = delete_node->next;
		list->head->prev = NULL;
		free(delete_node);
		list->length--;
		return index_data;
	}

	struct node *temp;
	temp = list->head;
	int i;
	for (i=1;i<=index;i++){
		temp = temp->next;
	}

	index_data = temp->data;
	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	free(temp);
	list->length--;



	return index_data;

}
