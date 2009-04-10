/*
 *  Showtime GTK frontend
 *  Copyright (C) 2009 Andreas Öman
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GU_H__
#define GU_H__

#include <gtk/gtk.h>

#include <prop.h>
#include <ui/ui.h>

LIST_HEAD(gu_nav_page_list, gu_nav_page);


typedef struct gtk_ui {
  uii_t gu_uii;
  
  hts_thread_t gu_thread;

  prop_courier_t *gu_pc;

  GtkWidget *gu_page_container;

  struct gu_nav_page_list gu_pages;

  struct gu_nav_page *gu_page_current;
  
  GtkWidget *gu_url;

} gtk_ui_t;


/**
 *
 */
typedef struct gu_nav_page {
  gtk_ui_t *gnp_gu;
  LIST_ENTRY(gu_nav_page) gnp_link;

  prop_t *gnp_prop;
  GtkWidget *gnp_rootbox;

  GtkWidget *gnp_view;
  GtkListStore *gnp_list_store;

} gu_nav_page_t;


void gu_nav_pages(struct prop_sub *s, prop_event_t event, ...);

void gu_directory_create(gu_nav_page_t *gnp);

#endif /* GU_H__ */
