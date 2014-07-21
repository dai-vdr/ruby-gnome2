# README

## Ruby-GNOME2 -- Ruby bindings for GNOME-2.x

This is a set of bindings for the GNOME-2.x and GNOME-3.x libraries to
use from Ruby 1.9.x, 2.0.0 and 2.1.

## Release packages

### ruby-gtk2

* Ruby/GLib2:           GLib 2.12.x or later
* Ruby/ATK:             ATK 1.12.x or later
* Ruby/Pango:           Pango 1.14.x or later
* Ruby/GdkPixbuf2:      GTK+ 2.0.x or later
* Ruby/GTK2:            GTK+ 2.10.x or later
* Ruby/GObjectIntrospection: GObject Introspection 1.32.1 or later

### ruby-gtk3

* ruby-gtk2 - Ruby/GTK2
* Ruby/GDK3:            GTK+ 3.4.2 or later
* Ruby/GTK3:            GTK+ 3.4.2 or later

### ruby-gnome2-all

* ruby-gtk2 + ruby-gtk3 - duplicated libraries
* Ruby/RSVG:            librsvg 2.8.0 or later
* Ruby/Poppler:         poppler-glib 0.8.0 or later
* Ruby/VTE              VTE 0.12.1 or later
* Ruby/GtkSourceView2:  GtkSourceView 2.0.0 or later
* Ruby/GIO2:            GIO 2.16.x or later
* Ruby/CairoGObject:    cairo-gobject 1.12.10 or later
* Ruby/Clutter:         Clutter 1.12.0 or later
* Ruby/ClutterGTK:      Clutter-GTK 1.2.0 or later
* Ruby/ClutterGStreamer:Clutter-GStreamer 2.0.0 or later
* Ruby/GtkSourceView3:  GtkSourceView 3.4.2 or later
* Ruby/VTE3:            VTE 0.32.2 or later
* Ruby/WebKitGTK:       WebKitGTK+ 1.8.1 or later (for GTK+ 3)
* Ruby/WebKitGTK2:      WebKitGTK+ 1.8.1 or later (for GTK+ 2)
* Ruby/GStreamer:       GStreamer 1.0.0 or later
* Ruby/GooCanvas:       GooCanvas 0.8.0 or later

### Experimental / Incomplete:

* ...

### Deprecated

* Ruby/PanelApplet:     gnome-panel 2.6.0 or later (deprecated since 0.16.0)
* Ruby/GConf2:          GConf 2.0.x or later
* Ruby/GNOME2:          libgnome-2.0.x, libgnomeui-2.0.x or later
* Ruby/GnomeCanvas2:    libgnomecanvas-2.0.x or later
* Ruby/GnomePrint:      libgnomeprint-2.8.x or later
* Ruby/GnomePrintUI:    libgnomeprintui-2.6.x or later
* Ruby/GnomeVFS:        GnomeVFS 2.0.x or later
* Ruby/GtkHtml2:        GtkHtml2 2.0.x or later
* Ruby/GtkGLExt:        GtkGLExt 1.0.3 or later
* Ruby/Libart2:         libart_lgpl 2.3.12 or later
* Ruby/GtkSourceView:   GtkSourceView 1.0.1 or later
* Ruby/Libglade2:       Libglade 2.0.x or later
* Ruby/GtkMozEmbed:     GtkMozEmbed (Mozilla 1.7.x or FireFox-1.0.x or later)

## Install

    % ruby extconf.rb
    % make
    % sudo make install

To compile and install a particular sub-binding, you can add arguments:

    % ruby extconf.rb [subdir]...
    e.g.) ruby extconf.rb glib2 pango atk gdk_pixbuf2 gtk2

Or you can compile each sub-binding:

    % cd <each sub-directory>
    % ruby extconf.rb
    % make
    % sudo make install

### extconf.rb options

<dl>
  <dt>--ruby</dt>
  <dd>ruby directory</dd>

  <dt>--topsrcdir</dt>
  <dd>top source directory</dd>

  <dt>--topdir</dt>
  <dd>top directory</dd>

  <dt>--strict</dt>
  <dd>
    if some libraries fail to compile/install, "make"
    command returns 1(exit 1)
  </dd>
</dl>

### Environment Variables

<dl>
  <dt>CAIRO_PATH</dt>
  <dd>rcairo path (for Win32).</dd>
</dl>

### Dependencies

You should install a sub-binding with its dependencies.  The
current dependencies are:

<dl>
  <dt>Ruby/GLib</dt>
  <dd>none.</dd>

  <dt>Ruby/ATK</dt>
  <dd>depends on Ruby/GLib.</dd>

  <dt>Ruby/Pango</dt>
  <dd>depends on Ruby/GLib, rcairo.</dd>

  <dt>Ruby/GdkPixbuf</dt>
  <dd>depends on Ruby/GLib.</dd>

  <dt>Ruby/GTK</dt>
  <dd>
    depends on Ruby/GLib, Ruby/ATK, Ruby/Pango,
    Ruby/GdkPixbuf, rcairo
  </dd>


  <dt>Ruby/RSVG</dt>
  <dd>depends on Ruby/GLib, Ruby/GdkPixbuf2.</dd>

  <dt>Ruby/Poppler</dt>
  <dd>depends on Ruby/GLib, Ruby/GdkPixbuf(optional), rcairo.</dd>

  <dt>Ruby/VTE</dt>
  <dd>depends on Ruby/GTK.</dd>

  <dt>Ruby/GtkSourceView2</dt>
  <dd>depends on Ruby/GTK.</dd>

  <dt>Ruby/GIO2</dt>
  <dd>depends on Ruby/GLib.</dd>
</dl>

### Experimental

<dl>
  <dt>Ruby/GStreamer</dt>
  <dd>depends on Ruby/GLib.</dd>

  <dt>Ruby/GooCanvas</dt>
  <dd>depends on Ruby/GTK.</dd>
</dl>

### Deprecated

<dl>
  <dt>Ruby/PanelApplet</dt>
  <dd>
    depends on Ruby/GTK (since libpanel-applet 2.25) or
    Ruby/GNOME (before libpanel-applet 2.25).
  </dd>

  <dt>Ruby/Libart</dt>
  <dd>depends on Ruby/GLib(Use mkmf-gnome2.rb only).</dd>

  <dt>Ruby/GConf</dt>
  <dd>depends on Ruby/GLib.</dd>

  <dt>Ruby/GNOME</dt>
  <dd>depends on Ruby/GnomeCanvas.</dd>

  <dt>Ruby/GnomeCanvas</dt>
  <dd>depends on Ruby/GTK, Ruby/Libart.</dd>

  <dt>Ruby/GnomeVFS</dt>
  <dd>depends on Ruby/GLib.</dd>

  <dt>Ruby/GnomePrint</dt>
  <dd>depends on Ruby/GLib, Ruby/Pango, Ruby/Libart2.</dd>

  <dt>Ruby/GnomePrintUI</dt>
  <dd>depends on Ruby/GTK, Ruby/GnomePrint.</dd>

  <dt>Ruby/GtkHtml2</dt>
  <dd>depends on Ruby/GTK.</dd>

  <dt>Ruby/GtkGLExt</dt>
  <dd>depends on Ruby/GTK, rbogl.</dd>

  <dt>Ruby/GtkSourceView</dt>
  <dd>depends on Ruby/GTK.</dd>

  <dt>Ruby/Libglade</dt>
  <dd>depends on Ruby/GTK, Ruby/GNOME(optional), REXML(optional).</dd>
</dl>

## Bugs

Please report bugs either in our bug tracker

* [https://github.com/ruby-gnome2/ruby-gnome2/issues](https://github.com/ruby-gnome2/ruby-gnome2/issues) (current)
* [http://sourceforge.net/tracker/?group_id=53614&atid=470969](http://sourceforge.net/tracker/?group_id=53614&atid=470969) (previous)

or on the ruby-gnome2-devel-en / ruby-gnome2-devel-ja mailing list

* [http://sourceforge.net/mail/?group_id=53614](http://sourceforge.net/mail/?group_id=53614)

## Copying

Copyright (c) 2002-2014 Ruby-GNOME2 Project Team

This program is free software.
You can distribute/modify this program under the terms of
the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1.

## Project Website

[http://ruby-gnome2.sourceforge.jp/](http://ruby-gnome2.sourceforge.jp/)
