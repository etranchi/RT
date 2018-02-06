/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_object2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdelanoe <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/06 15:23:14 by hdelanoe          #+#    #+#             */
/*   Updated: 2018/02/06 15:23:15 by hdelanoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		create_triangle(t_env *e, t_json *json)
{
	t_object	*triangle;
	t_json		*tmp;

	triangle = init_material();
	if (!(triangle->type = ft_strdup("triangle")))
		exit_rt(1);
	while(json->member)
	{
		tmp = json->member;
		if (!(ft_strcmp(tmp->name, "coord_1")) && tmp->member)
			triangle->point = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "coord_2")) && tmp->member)
			triangle->point_2 = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "coord_3")) && tmp->member)
			triangle->point_3 = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "color")) && tmp->member)
			triangle->color = parse_color(tmp->member);
		else if (!(ft_strcmp(tmp->name, "material")))
			parse_material(tmp, triangle);
		else
			ft_printf("{R}WARNING:{E} triangle as a bad attribut\n");
		json->member = json->member->next;
		free_json_member(&tmp);
	}
	debug_object(triangle);
	add_new_object(&e->object, triangle);
}

void		create_quad(t_env *e, t_json *json)
{
	t_object	*quad;
	t_json		*tmp;

	quad = init_material();
	if (!(quad->type = ft_strdup("quad")))
		exit_rt(1);
	while(json->member)
	{
		tmp = json->member;
		if (!(ft_strcmp(tmp->name, "coord_1")) && tmp->member)
			quad->point = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "coord_2")) && tmp->member)
			quad->point_2 = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "coord_3")) && tmp->member)
			quad->point_3 = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "coord_4")) && tmp->member)
			quad->point_4 = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "color")) && tmp->member)
			quad->color = parse_color(tmp->member);
		else if (!(ft_strcmp(tmp->name, "material")) && tmp->member)
			parse_material(tmp, quad);
		else
			ft_printf("{R}WARNING:{E} quad as a bad attribut\n");
		json->member = json->member->next;
		free_json_member(&tmp);
	}
	debug_object(quad);
	add_new_object(&e->object, quad);
}

void		create_torus(t_env *e, t_json *json)
{
	t_object	*torus;
	t_json 		*tmp;

	torus = init_material();
	if (!(torus->type = ft_strdup("torus")))
		exit_rt(1);
	while(json->member)
	{
		tmp = json->member;
		if (!(ft_strcmp(tmp->name, "coord")) && tmp->member)
			torus->center = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "sradius")) &&tmp->content)
			torus->radius = ft_atod(tmp->content);
		else if (!(ft_strcmp(tmp->name, "bradius")) && tmp->content)
			torus->lenght_max = ft_atod(tmp->content);
		else if (!(ft_strcmp(tmp->name, "axis")) && tmp->member)
			torus->axis = parse_point(tmp->member);
		else if (!(ft_strcmp(tmp->name, "color")) && tmp->member)
			torus->color = parse_color(tmp->member);
		else if (!(ft_strcmp(tmp->name, "material")))
			parse_material(tmp, torus);
		else
			ft_printf("{R}WARNING:{E} torus as a bad attribut\n");
		json->member = json->member->next;
		free_json_member(&tmp);
	}
	debug_object(torus);
	add_new_object(&e->object, torus);
}