#include "GuiControlSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"

GuiControlSlider::GuiControlSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue, const char* text)
    : GuiControl(GuiControlType::SLIDER, id), minValue(minValue), maxValue(maxValue), value(minValue)
{
    this->bounds = bounds;
    this->text = text;

    canClick = true;
    drawBasic = false;
    
    slider = app->tex->Load("Assets/Textures/Interfaz/slider.png");
    slider2 = app->tex->Load("Assets/Textures/Interfaz/sliderSelected.png");
    knob = app->tex->Load("Assets/Textures/Interfaz/knob.png");

    select_fx = app->audio->LoadAudioFx("get_item_fx");
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Update(float dt)
{
    

    return false;
}

bool GuiControlSlider::PostUpdate()
{
    if (state != GuiControlState::DISABLED)
    {
        app->input->GetMousePosition(mouseX, mouseY);

        mouseX *= app->win->GetScale();
        mouseY *= app->win->GetScale();

        // Update the state based on the mouse position
        if (selected)
        {
            state = GuiControlState::FOCUSED;

            /*if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
            }*/
            if (app->input->GetButton(LEFT) == KEY_REPEAT && value >= 0)
            {
                value--;
                app->audio->PlayTimedFx(select_fx, 500);

            }
            if (app->input->GetButton(RIGHT) == KEY_REPEAT && value <= 100)
            {
                value++;
                app->audio->PlayTimedFx(select_fx, 500);

            }
        }else{
            state = GuiControlState::NORMAL;
        }

        
        //// Update the value if the mouse is pressed and the slider is focused or pressed
        //if (state == GuiControlState::PRESSED || (state == GuiControlState::FOCUSED && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT))
        //{
        //    // Calculate the new value based on the mouse position
        //    float normalizedX = static_cast<float>(mouseX - bounds.x) / static_cast<float>(bounds.w);
        //    value = static_cast<int>(minValue + normalizedX * (maxValue - minValue));

        //    // Ensure the value stays within the valid range
        //    //value = std::min(std::max(value, minValue), maxValue);

        //    value = std::min(std::max(value, minValue), maxValue);
        //    NotifyObserver();
        //}

        // Draw the slider
        Draw();
    }

    return false;
}

void GuiControlSlider::Draw()
{
    
    //app->render->DrawRectangle(bounds, 150, 150, 150, 255, true, false);
    if (state == GuiControlState::NORMAL)
    {
        /*app->render->DrawTexture(slider, bounds.x + 50, bounds.y, SDL_FLIP_NONE, 0, 0, true);*/
    }
    if (state == GuiControlState::FOCUSED)
    {
        /*app->render->DrawTexture(slider2, bounds.x + 50, bounds.y, SDL_FLIP_NONE, 0, 0, true);*/
    }

    // Calculate the position of the slider knob based on the current value
    float normalizedValue = static_cast<float>(value - minValue) / static_cast<float>(maxValue - minValue);
    int knobX = bounds.x + 50 + static_cast<int>(normalizedValue * bounds.w);

    // Draw the slider knob
    
    SDL_Rect knobRect = { knobX , bounds.y - 5, 10, bounds.h + 10 };
    //app->render->DrawRectangle(knobRect, 255, 255, 0, 255, true, false);
   
    if (state == GuiControlState::FOCUSED)
    {
        app->render->DrawTexture(slider2, knobRect.x, knobRect.y, SDL_FLIP_NONE, 0, 0, true);
    }
    else
    {
        app->render->DrawTexture(knob, knobRect.x, knobRect.y, SDL_FLIP_NONE, 0, 0, true);
    }
   

    // Draw the text label
    /*app->render->DrawTextBound(text.GetString(), bounds.x, bounds.y - 40, 70, { 0,0,0,0});*/

}