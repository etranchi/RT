/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dguy-caz <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/04 20:12:37 by dguy-caz          #+#    #+#             */
/*   Updated: 2018/02/05 15:21:55 by hdelanoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color	choose_color(t_env *e)
{
	t_color new;

	if (e->aa_flag == 1 && e->pixelize == 0)
	{
		if (e->cel_shade == 1)
			new = e->current_color;
		else
			new = get_color(e);
	}
	else if (e->pixelize == 1)
	{
		if (e->edit_flag == 1)
			new = e->current_color;
		else if (e->cel_shade == 1)
			new = cel_shade_color(e);
		else
			new = get_color(e);
	}
	else if (e->edit_flag == 1)
		new = e->current_color;
	else if (e->cel_shade == 1)
		new = cel_shade_color(e);
	else
		new = get_color(e);
	return (new);
}

t_color	ambient_occlusion(t_env *e)
{
	t_color		c;
	t_rayon		origin;
	t_rayon		ray;
	t_vector	tmp;
	int			sample;

	c = set_color(1, 1, 1);
	origin.node = e->current_node;
	origin.normal = e->current_node_normal;
	sample = 16;
	e->hit = 0;
	while (sample > 0)
	{
		ray.rayon.x = RANDOM;
		ray.rayon.y = RANDOM;
		ray.rayon.z = RANDOM;
		ray.rayon = v_v_add(&ray.rayon, &origin.normal);
		ray.rayon = normalize(&ray.rayon);
		tmp = v_double_mult(&ray.rayon, 0.01);
		ray.origin = v_v_add(&origin.node, &tmp);
		e->hit = cast_ray(e, ray.rayon, ray.origin) ? e->hit + 1 : e->hit;
		sample--;
	}
	return (e->hit ? c_double_mult(&c, (double)1 / e->hit) : c);
}

double	get_specular(t_light *light, t_vector *view, t_vector *node)
{
	t_vector	tmp;
	t_vector	r;
	double		reflect;
	double		phong_color;

	reflect = dot_product(&light->rayon, node);
	tmp = v_double_mult(node, 2);
	r = v_double_mult(&tmp, reflect);
	r = v_v_subs(&r, &light->rayon);
	phong_color = 0.2 * powf(dot_product(view, &r), 50) * 3;
	return (phong_color);
}

t_color	add_diffuse(t_env *e, t_color *c, t_light *light, t_rayon *ray)
{
	t_color		diffuse;
	t_color		c_light;
	t_vector	angle;
	double		specular;

	diffuse = set_color(0, 0, 0);
	light->rayon = v_v_subs(&e->current_node, &light->origin);
	e->distance_light_object = magnitude(&light->rayon);
	light->rayon = normalize(&light->rayon);
	e->current_origin = light->origin;
	e->current_rayon = light->rayon;
	c_light = light_intersection(e, light);
	c_light = c_double_mult(&c_light, 1 - (e->distance_light_object / 100000));
	if (c_light.r == 0 && c_light.g == 0 && c_light.b == 0)
		return (*c);
	angle = v_double_mult(&light->rayon, (-1));
	light->angle = dot_product(&e->current_node_normal, &angle);
	specular = e->specular * get_specular(light, &ray->rayon, &ray->normal);
	if (light->angle > 0)
	{
		diffuse = c_c_mult(&e->current_color, &c_light);
		diffuse = c_double_add(&diffuse, specular);
		diffuse = c_double_mult(&diffuse, light->angle);
		diffuse = c_double_mult(&diffuse, e->diffuse);
	}
	return (c_c_add(c, &diffuse));
}

t_color	get_color(t_env *e)
{
	t_color	c;
	t_rayon	ray;
	t_light	*tmp_light;

	ray.origin = e->current_origin;
	ray.rayon = e->current_rayon;
	ray.node = e->current_node;
	ray.normal = e->current_node_normal;
	c = c_double_mult(&e->current_color, e->ambient);
	c = c_double_mult(&c, 1 - (e->distance / 100000));
	if ((c.r == 0 && c.g == 0 && c.b == 0)
		|| e->edit_flag == 1)
		return (c);
	tmp_light = e->light;
	while (tmp_light)
	{
		if (!ft_strcmp(tmp_light->type, "area_light"))
			c = get_area_light_intensity(e, tmp_light, &ray, &c);
		else
			c = add_diffuse(e, &c, tmp_light, &ray);
		tmp_light = tmp_light->next;
	}
	recurse_color(e, ray, &c);
	return (c);
}
