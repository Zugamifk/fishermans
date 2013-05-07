fontinfo *
font_init(const char *fname, color *c)
{
	fontinfo *fi = malloc(sizeof(fontinfo));
	fi->font = fontcharset_loadfont(fname);
	fi->col = c;
	return fi;
}

fontinfo *
font_initdef()
{
	fontinfo *fi = malloc(sizeof(fontinfo));
	fi->font = fontcharset_init();
	fi->col = color_new3(0, 0, 0);
	return fi;
}

void
font_draw(char *str, fontinfo *fi, double x, double y, double s, double w)
{
	if (!fi) {
		fi = &GlobalFontInfo;
	}

	if (!fi->font) {
		fi->font = GlobalFontInfo.font;
	}
	
	glPushMatrix();
	color_apply(fi->col);
	glTranslated(x, y, 0.0);
	glScaled(s, s, s);

	x = 0.0;
	while(*str!='\0') {
		if (*str==' ') {
			x += 0.6+fi->font->spacing;
			glTranslated(0.6+fi->font->spacing, 0.0,0.0);
			
			char next[256];
			sscanf(str, "%s", next);
			if (sscanf > 0 && x + font_getstrwidth(next, fi) > w) {
				glTranslated(-x, -1.4, 0.0);
				x = 0.0;
			}

			str++;
			
			continue;
		} else 
		if (*str=='\n'){
			glTranslated(-x, -1.4, 0.0);
			x = 0.0;
			str++;
			continue;
		}
		
		fontchar *c = fontcharset_getchar(fi->font, str);
		fontchar_draw(c);
		if (c) {
			glTranslated(c->width+fi->font->spacing, 0.0,0.0);
			x += c->width+fi->font->spacing;
		} else {
			glTranslated(1.0+fi->font->spacing, 0.0,0.0);
			x += 1.0+fi->font->spacing;
		}
		str++;
	}
	glPopMatrix();
}

double
font_getstrwidth(char *str, fontinfo *fi)
{
	if (!fi) {
		fi = &GlobalFontInfo;
	}

	if (!fi->font) {
		fi->font = GlobalFontInfo.font;
	}
	
	double len = 0.0;
	
	while(*str!='\0') {
		if (*str==' ') {
			len += 0.6 + fi->font->spacing;
			str++;
			continue;
		}
		
		fontchar *c = fontcharset_getchar(fi->font, str);
		if (c)
			len += c->width + fi->font->spacing;
		else
			len += 1.0 + fi->font->spacing;
			
		str++;
	}
	
	return len;
}
