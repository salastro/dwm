static int
width_seltagicon(Bar *bar, BarArg *a)
{
    Monitor *m = bar->mon;
    unsigned int occ = m->tagset[m->seltags];
    int i;

    /* Find the first selected tag */
    for (i = 0; i < NUMTAGS && !(occ & (1 << i)); i++)
        ;
    if (i >= NUMTAGS)
        return 0;

    return TEXTW(tagicon(m, i));
}

static int
draw_seltagicon(Bar *bar, BarArg *a)
{
    Monitor *m = bar->mon;
    unsigned int occ = m->tagset[m->seltags];
    int i;
    char *icon;

    /* Find the first selected tag */
    for (i = 0; i < NUMTAGS && !(occ & (1 << i)); i++)
        ;
    if (i >= NUMTAGS)
        return 0;

    icon = tagicon(m, i);
    drw_setscheme(drw, scheme[SchemeTagsNorm]);
    drw_text(drw, a->x, a->y, a->w, a->h, lrpad / 2, icon, 0, False);

    return 1;
}

static int
click_seltagicon(Bar *bar, Arg *arg, BarArg *a)
{
    return ClkTagBar;
}
