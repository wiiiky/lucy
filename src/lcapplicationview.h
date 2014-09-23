/* lcapplicationview.h generated by valac 0.20.1, the Vala compiler, do not modify */


#ifndef __LCAPPLICATIONVIEW_H__
#define __LCAPPLICATIONVIEW_H__

#include <glib.h>
#include <gtk/gtk.h>
#include "lcprotocol.h"

G_BEGIN_DECLS
#define TYPE_LC_APPLICATION_VIEW (lc_application_view_get_type ())
#define LC_APPLICATION_VIEW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_LC_APPLICATION_VIEW, LcApplicationView))
#define LC_APPLICATION_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_LC_APPLICATION_VIEW, LcApplicationViewClass))
#define IS_LC_APPLICATION_VIEW(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_LC_APPLICATION_VIEW))
#define IS_LC_APPLICATION_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_LC_APPLICATION_VIEW))
#define LC_APPLICATION_VIEW_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_LC_APPLICATION_VIEW, LcApplicationViewClass))
typedef struct _LcApplicationView LcApplicationView;
typedef struct _LcApplicationViewClass LcApplicationViewClass;
typedef struct _LcApplicationViewPrivate LcApplicationViewPrivate;

struct _LcApplicationView {
    GtkScrolledWindow parent_instance;
    LcApplicationViewPrivate *priv;
};

struct _LcApplicationViewClass {
    GtkScrolledWindowClass parent_class;
};


GType lc_application_view_get_type(void) G_GNUC_CONST;
LcApplicationView *lc_application_view_new(void);
LcApplicationView *lc_application_view_construct(GType object_type);

/*
 * 彻底更新应用列表，该函数保证显示内容与list一致，不多不少
 */
void lc_application_view_update(LcApplicationView * self, GList * list);

/*
 * 在应用列表中增加一项，不会检查是否重复
 */
void lc_application_view_append(LcApplicationView * self,
                                LcProtocolApplication * data);


G_END_DECLS
#endif
