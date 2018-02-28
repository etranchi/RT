/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrap_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: notraore <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/08 13:07:11 by notraore          #+#    #+#             */
/*   Updated: 2018/02/26 20:20:04 by hdelanoe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		wrap_sphere(t_env *e, t_object *object)
{
	t_vector			vp;
	double				phi;
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
	e->u = (acos(-dot_product(&vp, &vecdirx) / sin(phi))) / (2.0 * PI);
	if (!(dot_product(&cross, &vp) > 0.0))
		e->u = 1.0 - e->u;
	e->i = (int)(e->v * (e->bpp[e->ti]));
	e->j = (int)(e->u * e->sl[e->ti] - e->pet);
	e->current_color = get_texture_info(e->tx_dta[e->ti],
			e, e->sl[e->ti]);
}

void		wrap_plane(t_env *e)
{
	int carre;

	carre = floor(e->current_node.x) + floor(e->current_node.z);
	if (carre % 2 == 0)
		e->current_color = set_color(0, 0, 0);
	else
		e->current_color = set_color(0.6184, 0.54561, 0.3481);
}

void		wrap_cylinder(t_env *e, t_object *object)
{
	t_vector	d;
	t_vector	mult;

	mult = v_v_mult(&object->center, &object->axis);
	d = v_v_subs(&mult, &e->current_node);
	e->u = 0.5 + atan2(d.z, d.x) / PI * 0.5;
	e->v = d.y / -object->lenght_max;
	e->v = e->v - floor(e->v);
	e->i = max((e->u * e->bpp[e->ti]), 0);
	e->j = max((e->v * e->sl[e->ti]), 0);
	e->current_color = get_texture_info(e->tx_dta[e->ti], e,
			e->sl[e->ti]);
}

void		wrap_cone(t_env *e, t_object *object)
{
	t_vector			vp;
	double				phi;
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
	e->u = (acos(-dot_product(&vp, &vecdirx) / sin(phi))) / (2.0 * PI);
	if (dot_product(&cross, &vp) > 0.0)
		e->u = e->u;
	else
		e->u = 1.0 - e->u;
	e->i = (int)(e->v * e->bpp[e->ti]);
	e->j = (int)(e->u * e->sl[e->ti]);
	e->current_color = get_texture_info(e->tx_dta[e->ti],
			e, e->sl[e->ti]);
}
