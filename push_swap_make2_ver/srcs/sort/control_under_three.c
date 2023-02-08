/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_under_three.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hwankim <hwankim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:03:48 by hwankim           #+#    #+#             */
/*   Updated: 2023/02/07 14:15:09 by hwankim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_input_two(t_stack *a, t_stack *b, int flag)
{
	if (flag == A)
	{
		if (a->top->value > a->top->next->value)
			swap_stack(a, A);
	}
	else
	{
		if (b->top->value < b->top->next->value)
			swap_stack(b, B);
		move_stack(b, a, A);
		move_stack(b, a, A);
	}
}

void	control_under_three(int r, t_stack *a, t_stack *b, int flag)
{
	if (r == 3)
	{
		if (flag == A)
			sort_input_three_a(r, a);
		else
			sort_input_three_b(r, a, b);
	}
	else if (r == 2)
		sort_input_two(a, b, flag);
	else if (r == 1)
	{
		if (flag == B)
			move_stack(b, a, A);
	}
}
