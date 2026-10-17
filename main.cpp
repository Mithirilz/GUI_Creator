#include <raylib.h>
#include <string>
#include <vector>

struct Button{
    Vector2 pos;
    const int width;
    const int height;
    Color colour;

    bool is_dragged = false;
    Vector2 mouse_offset;

    bool is_left_clicked(const Vector2& mouse_pos){
        Rectangle button{pos.x, pos.y, static_cast<float>(width), static_cast<float>(height)};

        if(CheckCollisionPointRec(mouse_pos, button) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
            return true;
        } else{
            return false;
        }
    }

    bool is_right_clicked(const Vector2& mouse_pos){
        Rectangle button{pos.x, pos.y, static_cast<float>(width), static_cast<float>(height)};

        if(CheckCollisionPointRec(mouse_pos, button) && IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
            return true;
        } else{
            return false;
        }
    }

    void draw(Vector2 mouse_pos){
        if(is_left_clicked(mouse_pos)){
            colour = GREEN;
        } else{
            colour = RED;
        }

        if(!is_dragged){
            if(is_right_clicked(mouse_pos)){
                is_dragged = true;

                mouse_offset.x = mouse_pos.x - pos.x;
                mouse_offset.y = mouse_pos.y - pos.y;
            }
        } else if(is_dragged){
            pos.x = mouse_pos.x - mouse_offset.x;
            pos.y = mouse_pos.y - mouse_offset.y;
        }

        if(IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)){
            is_dragged = false;
        }

        DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), width, height, colour);
    }

};

struct Textbox{
    Vector2 box_pos;
    const int box_width;
    const int box_height;

    Color box_colour;
    Color text_colour;

    std::string text;
    bool is_typing = false;

    Color flicker_colour;
    float flicker_brightness = 1.0;
    int counter = 0;

    bool is_left_clicked(const Vector2& mouse_pos){
        Rectangle button{box_pos.x, box_pos.y, static_cast<float>(box_width), static_cast<float>(box_height)};

        if(CheckCollisionPointRec(mouse_pos, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            return true;
        } else{
            return false;
        }
    }

    bool is_right_clicked(const Vector2& mouse_pos){
        Rectangle button{box_pos.x, box_pos.y, static_cast<float>(box_width), static_cast<float>(box_height)};

        if(CheckCollisionPointRec(mouse_pos, button) && IsMouseButtonDown(MOUSE_RIGHT_BUTTON)){
            return true;
        } else{
            return false;
        }
    }

    void draw(const Vector2 mouse_pos, const int& key_pressed){
        if(!is_typing){
            if(is_left_clicked(mouse_pos)){
                is_typing = true;
                box_colour = Fade(RAYWHITE, 0.5f);
            }
        } else{ 
            flicker_colour = ColorAlpha(WHITE, 1 - flicker_brightness);

            if(counter != 1500){
                counter++;
            } else{
                counter = 0;
                flicker_brightness = 1 - flicker_brightness;
            }

            if(is_left_clicked(mouse_pos)){
                is_typing = false;
                box_colour = RAYWHITE;
                flicker_colour = ColorAlpha(WHITE, 0);
            } else if(static_cast<char>(key_pressed) != 0){
                text.push_back(static_cast<char>(key_pressed));
            } else if(IsKeyPressed(KEY_BACKSPACE) && !text.empty()){
                text.pop_back();
            }
        }

        const int text_width = MeasureText(text.c_str(), box_height/2);
        const int start_of_text_x = static_cast<int>(box_pos.x) + (box_width - text_width)/2;
        const int start_of_text_y = static_cast<int>(box_pos.y) + 13;

        DrawRectangle(static_cast<int>(box_pos.x), static_cast<int>(box_pos.y), static_cast<int>(box_width), static_cast<int>(box_height), box_colour);
        DrawText(text.c_str(), start_of_text_x, start_of_text_y, box_height/2, text_colour);
        DrawLine((start_of_text_x + text_width) + 5, start_of_text_y, (start_of_text_x + text_width) + 5, start_of_text_y + box_height/2, flicker_colour);
    }
};

struct Slider{
    Vector2 line_start;
    Vector2 line_end;

    Vector2 circle_center;
    float circle_radius;

    Color line_colour;
    Color circle_colour;

    bool is_sliding = false;

    bool is_left_clicked(const Vector2& mouse_pos){
        if(CheckCollisionPointCircle(mouse_pos, circle_center, circle_radius) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            return true;
        } else{
            return false;
        }
    }

    void draw(const Vector2 mouse_pos){
        if(!is_sliding){
            if(is_left_clicked(mouse_pos)){
                is_sliding = true;
            }
        } else{
            circle_center.x += (mouse_pos.x - circle_center.x);
        }

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
            is_sliding = false;
        }

        if(circle_center.x < line_start.x){
            circle_center.x = line_start.x;
        } else if(circle_center.x > line_end.x){
            circle_center.x = line_end.x;
        }

        DrawLine(static_cast<int>(line_start.x), static_cast<int>(line_start.y), static_cast<int>(line_end.x), static_cast<int>(line_end.y), line_colour);
        DrawCircle(static_cast<int>(circle_center.x), static_cast<int>(circle_center.y), circle_radius, circle_colour);
    }
};

struct GUI{
    std::vector<Button> button_list;
    std::vector<Textbox> textbox_list;

    void instantiate_button(Vector2 mouse_pos){
        mouse_pos.x -= 100.0/2;
        mouse_pos.y -= 50.0/2;

        button_list.emplace_back(mouse_pos, 100, 50, RED);
    }

    void instantiate_textbox(Vector2 mouse_pos){
        mouse_pos.x -= 100.0/2;
        mouse_pos.y -= 50.0/2;

        textbox_list.emplace_back(mouse_pos, 100, 50, RAYWHITE, ORANGE);
    }

    void draw_all(const Vector2 mouse_pos){
        if(!button_list.empty()){
            for(int i = 0; i < button_list.size(); i++){
                button_list[i].draw(mouse_pos);
            }
        }

        if(!textbox_list.empty()){ 
            int key_pressed = GetCharPressed();

            for(int i = 0; i < textbox_list.size(); i++){
                textbox_list[i].draw(mouse_pos, key_pressed);
            }
        }
    }
};

int main(){ 
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    const int window_width = 1280;
    const int window_height = 720;
    InitWindow(window_width, window_height, "GUI_WIZARD");
    ShowCursor();
    GUI GUI_manager; 

    Vector2 button_pos;
    const int button_width = 100;
    const int button_height = 50;
    button_pos.x = static_cast<float>(window_width)/2 - static_cast<float>(button_width)/2;
    button_pos.y = static_cast<float>(window_height)/2 - static_cast<float>(button_height)/2;

    Button button1{button_pos, button_width, button_height, RED};
    Slider slider_entity{Vector2{button_pos.x, button_pos.y + 100}, Vector2{button_pos.x + button_width, button_pos.y +100}, Vector2{button_pos.x, button_pos.y + 100}, 8, BLUE, RAYWHITE};

    Vector2 mouse_pos;

    while(!WindowShouldClose()){
        mouse_pos = GetMousePosition();

        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Q)){
            GUI_manager.instantiate_button(mouse_pos);
        }

        if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_W)){
            GUI_manager.instantiate_textbox(mouse_pos);
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            GUI_manager.draw_all(mouse_pos);
            button1.draw(mouse_pos);
            slider_entity.draw(mouse_pos);
        EndDrawing();
    }
}