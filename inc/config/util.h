#define foreach(i, c) for( __typeof( (c).begin() ) i = (c).begin(); i != (c).end(); ++i )

#define eatwhite(x) {while (strchr(" \t\r\n", (x).peek())) (x).get();}
