typedef struct _Button Button;
Button* mbar_create_button(BarApp* app, const char* text);
void mbar_free_button(Button* value);
