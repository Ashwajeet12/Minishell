// // // int insert_at_last(Slist **head, data_t data)
// // // {
// // // 	/* create new node */
// // // 	Slist *new = malloc(sizeof(Slist));
// // // 	if (!new)
// // // 	{
// // // 		printf("Failure In single linked list creation\n");
// // // 	}

// // // 	/* if new node created, update data + link */
// // // 	new->data = data;
// // // 	new->link = NULL;

// // // 	/* If list is empty */
// // // 	if (*head == NULL)
// // // 	{
// // // 		/* add new node to first position */
// // // 		*head = new;
// // // 		//return SUCCESS;
// // // 	}

// // // 	/* else */
// // // 	/* temp points to first node */
// // // 	Slist *temp = *head;

// // // 	/* traverse to next node check next node is present or not */
// // // 	while (temp->link != NULL)
// // // 	{
// // // 		temp = temp->link;
// // // 	}

// // // 	/* establish the link between last and new node*/
// // // 	temp->link = new;

// // // 	//return SUCCESS;
// // // }



// int sl_delete_last(Slist **head)
// {
//     if(*head == NULL)
//     {
//         return FAILURE;
//     }
    
//     if((*head) -> link == NULL)
//     {
//         free(*head);
//         *head = NULL;
//         return SUCCESS;
//     }
    
//     Slist *temp = *head;
    
//     while(temp -> link -> link != NULL)
//     {
//         temp = temp -> link;
//     }
    
//     free(temp -> link);
//     temp -> link = NULL;
    
//     return SUCCESS;
// }

// // void print_list(Slist *head)
// // {
// // 	if (head == NULL)
// // 	{
// // 		printf("INFO : List is empty\n");
// // 	}
// //     else
// //     {
// // 	    while (head)		
// // 	    {
// // 		    printf("%d -> ", head -> data);
// // 		    head = head -> link;
// // 	    }

// // 	    printf("NULL\n");
// //     }
// // }












// // // INSERT AT FIRST
// // int insert_at_first(Slist **head, data_t data)
// // {
// // 	Slist *first = malloc(sizeof(Slist));
// // 	if(first == NULL)
// // 	{
// // 	    return FAILURE;
// // 	}
	
// // 	first -> data = data;
// // 	first -> link = *head;
	
// // 	*head = first;
	
// // }