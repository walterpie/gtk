/*
 * Copyright © 2020 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Matthias Clasen <mclasen@redhat.com>
 */

#include "config.h"

#include "gdkpopupprivate.h"

/**
 * SECTION:gdkpopup
 * @Short_description: Interface for popup surfaces
 * @Title: Popups
 *
 * A #GdkPopup is a surface that is attached to another surface,
 * and is positioned relative to it.
 */


/* FIXME: this can't have GdkSurface as a prerequisite
 * as long as GdkSurface implements this interface itself
 */
G_DEFINE_INTERFACE (GdkPopup, gdk_popup, G_TYPE_OBJECT)

static gboolean
gdk_popup_default_present (GdkPopup       *popup,
                           int             width,
                           int             height,
                           GdkPopupLayout *layout)
{
  return FALSE;
}

static GdkGravity
gdk_popup_default_get_surface_anchor (GdkPopup *popup)
{
  return GDK_GRAVITY_STATIC;
}

static GdkGravity
gdk_popup_default_get_rect_anchor (GdkPopup *popup)
{
  return GDK_GRAVITY_STATIC;
}

static GdkSurface *
gdk_popup_default_get_parent (GdkPopup *popup)
{
  return NULL;
}

static void
gdk_popup_default_get_position (GdkPopup *popup,
                                int      *x,
                                int      *y)
{
  *x = 0;
  *y = 0;
}

static gboolean
gdk_popup_default_get_autohide (GdkPopup *popup)
{
  return FALSE;
}

static void
gdk_popup_default_init (GdkPopupInterface *iface)
{
  iface->present = gdk_popup_default_present;
  iface->get_surface_anchor = gdk_popup_default_get_surface_anchor;
  iface->get_rect_anchor = gdk_popup_default_get_rect_anchor;
  iface->get_parent = gdk_popup_default_get_parent;
  iface->get_position = gdk_popup_default_get_position;
  iface->get_autohide = gdk_popup_default_get_autohide;
}

/**
 * gdk_popup_present:
 * @popup: the #GdkPopup to show
 * @width: the unconstrained popup width to layout
 * @height: the unconstrained popup height to layout
 * @layout: the #GdkPopupLayout object used to layout
 *
 * Present @popup after having processed the #GdkPopupLayout rules.
 * If the popup was previously now showing, it will be showed,
 * otherwise it will change position according to @layout.
 *
 * After calling this function, the result of the layout can be queried
 * using gdk_popup_get_position(), gdk_surface_get_width(),
 * gdk_surface_get_height(), gdk_popup_get_rect_anchor() and
 * gdk_popup_get_surface_anchor().
 *
 * Presenting may have fail, for example if it was immediately
 * hidden if the @popup was set to autohide.
 *
 * Returns: %FALSE if it failed to be presented, otherwise %TRUE.
 */
gboolean
gdk_popup_present (GdkPopup       *popup,
                   int             width,
                   int             height,
                   GdkPopupLayout *layout)
{
  g_return_val_if_fail (GDK_IS_POPUP (popup), FALSE);
  g_return_val_if_fail (width > 0, FALSE);
  g_return_val_if_fail (height > 0, FALSE);
  g_return_val_if_fail (layout != NULL, FALSE);

  return GDK_POPUP_GET_IFACE (popup)->present (popup, width, height, layout);
}

/**
 * gdk_popup_get_surface_anchor:
 * @popup: a #GdkPopup
 *
 * Gets the current popup surface anchor.
 *
 * The value returned may change after calling gdk_popup_present(),
 * or after the "popup-layout-changed" is emitted.
 *
 * Returns: the current surface anchor value of @popup
 */
GdkGravity
gdk_popup_get_surface_anchor (GdkPopup *popup)
{
  g_return_val_if_fail (GDK_IS_POPUP (popup), GDK_GRAVITY_STATIC);

  return GDK_POPUP_GET_IFACE (popup)->get_surface_anchor (popup);
}

/**
 * gdk_popup_get_rect_anchor:
 * @popup: a #GdkPopup
 *
 * Gets the current popup rectangle anchor.
 *
 * The value returned may change after calling gdk_popup_present(),
 * or after the "popup-layout-changed" is emitted.
 *
 * Returns: the current rectangle anchor value of @popup
 */
GdkGravity
gdk_popup_get_rect_anchor (GdkPopup *popup)
{
  g_return_val_if_fail (GDK_IS_POPUP (popup), GDK_GRAVITY_STATIC);

  return GDK_POPUP_GET_IFACE (popup)->get_rect_anchor (popup);
}

/**
 * gdk_popup_get_parent:
 * @popup: a #GdkPopup
 *
 * Returns the parent surface of a popup.
 *
 * Returns: (transfer none): the parent @popup
 */
GdkSurface *
gdk_popup_get_parent (GdkPopup *popup)
{
  g_return_val_if_fail (GDK_IS_POPUP (popup), NULL);

  return GDK_POPUP_GET_IFACE (popup)->get_parent (popup);
}

/**
 * gdk_popup_get_position:
 * @popup: a #GdkPopup
 * @x: (out): X coordinate of popup
 * @y: (out): Y coordinate of popup
 *
 * Obtains the position of the popup relative to its parent.
 */
void
gdk_popup_get_position (GdkPopup *popup,
                        int      *x,
                        int      *y)
{
  g_return_if_fail (GDK_IS_POPUP (popup));

  GDK_POPUP_GET_IFACE (popup)->get_position (popup, x, y);
}

/**
 * gdk_popup_get_autohide:
 * @popup: a #GdkPopup
 *
 * Returns whether this popup is set to hide on outside clicks.
 *
 * Returns: %TRUE if @popup will autohide
 */
gboolean
gdk_popup_get_autohide (GdkPopup *popup)
{
  g_return_val_if_fail (GDK_IS_POPUP (popup), FALSE);

  return GDK_POPUP_GET_IFACE (popup)->get_autohide (popup);
}
