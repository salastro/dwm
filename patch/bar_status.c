int
width_status(Bar *bar, BarArg *a)
{
	return TEXTWM(stext);
}

int
width_status_es(Bar *bar, BarArg *a)
{
	return TEXTWM(estext) - lrpad;
}

int
width_status_rs(Bar *bar, BarArg *a)
{
	return TEXTWM(rstext) - lrpad;
}

int
draw_status(Bar *bar, BarArg *a)
{
	return drw_text(drw, a->x, a->y, a->w, a->h, lrpad / 2, stext, 0, True);
}

int
draw_status_es(Bar *bar, BarArg *a)
{
	return drw_text(drw, a->x, a->y, a->w, a->h, 0, estext, 0, True);
}

int
draw_status_rs(Bar *bar, BarArg *a)
{
	return drw_text(drw, a->x, a->y, a->w, a->h, 0, rstext, 0, True);
}

int
click_status(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkStatusText;
}

