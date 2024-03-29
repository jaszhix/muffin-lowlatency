/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

#ifndef META_COMPOSITOR_PRIVATE_H
#define META_COMPOSITOR_PRIVATE_H

#include <X11/extensions/Xfixes.h>

#include <meta/compositor.h>
#include <meta/display.h>
#include "meta-plugin-manager.h"
#include "meta-window-actor-private.h"
#include <clutter/clutter.h>

struct _MetaCompositor
{
  MetaDisplay    *display;
  MetaScreen     *screen;

  Atom            atom_x_root_pixmap;
  Atom            atom_x_set_root;
  Atom            atom_net_wm_window_opacity;
  guint           pre_paint_func_id;
  guint           post_paint_func_id;

  ClutterActor   *stage, *shadow_src;
  ClutterActor   *bottom_window_group, *window_group, *overlay_group, *top_window_group;
  ClutterActor   *background_actor;
  ClutterActor   *hidden_group;

  GList          *windows;

  MetaWindowActor *unredirected_window;
  MetaWindowActor *top_window_actor;

  CoglContext    *context;

  Window          output;

  /* Used for unredirecting fullscreen windows */
  guint           disable_unredirect_count;

  /* Before we create the output window */
  XserverRegion   pending_input_region;

  gint            switch_workspace_in_progress;

  MetaPluginManager *plugin_mgr;

  MetaPlugin     *modal_plugin;

  gint64          server_time_query_time;
  gint64          server_time_offset;

  guint           server_time_is_monotonic_time : 1;
  guint           show_redraw : 1;
  guint           debug       : 1;
  guint           no_mipmaps  : 1;
  guint           override_window_on_top : 1;

  gboolean frame_has_updated_xsurfaces;
  gboolean have_x11_sync_object;
  gboolean popup_window_visible;
};

/* Wait 2ms after vblank before starting to draw next frame */
#define META_SYNC_DELAY 0

CoglContext * meta_compositor_get_cogl_context (void);

void meta_switch_workspace_completed (MetaScreen    *screen);

MetaWindow * meta_compositor_get_window_for_xwindow (Window xwindow);

gboolean meta_begin_modal_for_plugin (MetaScreen       *screen,
                                      MetaPlugin       *plugin,
                                      Window            grab_window,
                                      Cursor            cursor,
                                      MetaModalOptions  options,
                                      guint32           timestamp);
void     meta_end_modal_for_plugin   (MetaScreen       *screen,
                                      MetaPlugin       *plugin,
                                      guint32           timestamp);

gint64 meta_compositor_monotonic_time_to_server_time (MetaDisplay *display,
                                                      gint64       monotonic_time);

void meta_compositor_grab_op_begin (MetaCompositor *compositor);
void meta_compositor_grab_op_end (MetaCompositor *compositor);

void meta_check_end_modal (MetaScreen *screen);

void meta_compositor_update_sync_state (MetaCompositor *compositor,
                                        MetaSyncMethod  method);

void meta_compositor_set_all_obscured (MetaCompositor *compositor,
                                       gboolean        obscured);

#endif /* META_COMPOSITOR_PRIVATE_H */
