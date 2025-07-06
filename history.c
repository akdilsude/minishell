/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakdil <sakdil@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:46:38 by segunes           #+#    #+#             */
/*   Updated: 2025/05/02 15:59:55 by sakdil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}

static void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}

void	add_to_history(t_list **history, char *input)
{
	t_list	*new_node;

	new_node = ft_lstnew(ft_strdup(input));
	if	(!new_node)
	{
		perror("Failed to allocate memory for history");
		return;
	}
	ft_lstadd_back(history, new_node);
}

void print_history(t_list *history)
{
	t_list	*current;
	int		i;

	i = 1;
	current = history;
	while (current)
	{
		if (current->content)
			printf("%d\t%s\n", i, (char *)current->content);
		current = current->next;
		i++;
	}
}
