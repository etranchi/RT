/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notraore <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/08 13:07:11 by notraore          #+#    #+#             */
/*   Updated: 2017/12/08 13:07:13 by notraore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_color		normalize_color(t_color *color)
{
	t_color new;

	new.r = color->r / 255;
	new.g = color->g / 255;
	new.b = color->b / 255;
	return (new);
}

void		load_texture(t_env *e)
{
	if (!((e->text_img = mlx_xpm_file_to_image(e->mlx.mlx_ptr,
	"./textures/metal.xpm", &(e->sl), &e->bpp))))
		ft_kill("Texture error");
	e->text_data = (unsigned char*)mlx_get_data_addr(e->text_img, &e->bpp,
	&e->naz, &e->end);
}

void		wrap_sphere(t_env *e, t_object *object)
{
	t_vector			vp;
	double				phi;
	double				theta;
	t_vector			vecdirx;
	t_vector			vecdiry;
	t_vector			cross;

	vecdirx = (t_vector){1, 0, 0};
	vecdiry = (t_vector){0, 1, 0};
	cross = v_v_mult(&vecdiry, &vecdirx);
	vp = v_v_subs(&object->center, &e->current_node);
	vp = normalize(&vp);
	phi = acos(-dot_product(&vecdiry, &vp));
	e->v = phi / PI;
	theta = (acos(-dot_product(&vp, &vecdirx) / sin(phi))) / (2.0 * PI);
	if (dot_product(&cross, &vp) > 0.0)
		e->u = theta;
	else
		e->u = 1.0 - theta;
	e->i = (int)(e->v * (2592));
	e->j = (int)(e->u * e->sl);
	e->current_color = get_texture_info(e->text_data, e);
}

void		wrap_plane(t_env *e)//, t_object *object)
{
	int carre;

	// if (!ft_strcmp(object->tex_type, "square"))
	// 	carre = floor(e->current_node.x) + floor(e->current_node.z);
	// else if (!ft_strcmp(object->tex_type, "columns"))
		carre = floor(e->current_node.x);
	// else if (!ft_strcmp(object->tex_type, "lines"))
		// carre = floor(e->current_node.z);
	if (carre % 2 == 0)
		e->current_color = set_color(0, 0, 0);
	else
		e->current_color = set_color(0.6184, 0.54561, 0.3481);
}

void		wrap_cone(t_env *e, t_object *object)
{
	t_vector			vp;
	double				phi;
	double				theta;
	t_vector			vecdirx;
	t_vector			vecdiry;
	t_vector			cross;

	vecdirx = (t_vector){1, 0, 0};
	vecdiry = (t_vector){0, 1, 0};
	cross = v_v_mult(&vecdiry, &vecdirx);
	vp = v_v_subs(&object->center, &e->current_node);
	vp = normalize(&vp);
	phi = acos(-dot_product(&vecdiry, &vp));
	e->v = phi / PI;
	theta = (acos(-dot_product(&vp, &vecdirx) / sin(phi))) / (2.0 * PI);
	if (dot_product(&cross, &vp) > 0.0)
		e->u = theta;
	else
		e->u = 1.0 - theta;
	e->i = (int)(e->v * 2096);
	e->j = (int)(e->u * e->sl);
	e->current_color = get_texture_info(e->text_data, e);
}

t_vector	v_v_mlt(t_vector *a, t_vector *b)
{
	t_vector	r;

	r.x = a->x * b->x;
	r.y = a->y * b->y;
	r.z = a->z * b->z;
	return (r);
}

int			max(int a, int b)
{
	return (a > b ? a : b);
}

void		wrap_cylinder(t_env *e, t_object *object)
{
	t_vector	d;
	t_vector	mult;

	mult = v_v_mlt(&object->center, &object->axis);
	d = v_v_subs(&mult, &e->current_node);
	e->u = 0.5 + atan2(d.z, d.x) / PI * 0.5;
	e->v = d.y / (0 - object->lenght_max);
	e->v = e->v - floor(e->v);
	e->i = max((e->u * 2592), 0);
	e->j = max((e->v * e->sl), 0);
	e->current_color = get_texture_info(e->text_data, e);
}

t_color		get_texture_info(unsigned char *text_data, t_env *e)
{
	int			nb;
	t_color		clr;

	nb = (e->j * 4) + (e->i * e->sl * 4);
	clr.b = text_data[nb];
	clr.g = text_data[nb + 1];
	clr.r = text_data[nb + 2];
	clr = normalize_color(&clr);
	return (clr);
}
