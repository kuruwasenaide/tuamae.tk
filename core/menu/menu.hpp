#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/dx9/imgui_impl_dx9.h"
#include "../menu/config/config.hpp"

extern IDirect3DStateBlock9* state_block;

class c_menu {
public:
	void run();

	void __stdcall create_objects(IDirect3DDevice9* device) {
		if (hooks::window)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	void __stdcall invalidate_objects() {
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void __stdcall setup_resent(IDirect3DDevice9* device) {
		ImGui_ImplDX9_Init(hooks::window, device);

		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2(0, 0);
		style.WindowMinSize = ImVec2(32, 32);
		style.WindowRounding = 0;
		style.WindowTitleAlign = ImVec2(0.5f, 0.45f);
		style.ChildWindowRounding = 0.0f;
		style.FramePadding = ImVec2(4, 3);
		style.FrameRounding = 0.0f;
		style.ItemSpacing = ImVec2(8, 8);
		style.ItemInnerSpacing = ImVec2(8, 8);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 0.0f;
		style.ScrollbarSize = 5.0f;
		style.ScrollbarRounding = 50.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 0.0f;
		style.ButtonTextAlign = ImVec2(0.5f, 0.45f);
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.CurveTessellationTol = 1.f;

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);
		colors[ImGuiCol_Border] = ImVec4(.05, .05, .05, .25);
		colors[ImGuiCol_Header] = ImVec4(.080, .080, .080, 1);
		colors[ImGuiCol_HeaderActive] = ImVec4(.095, .095, .095, 1);
		colors[ImGuiCol_FrameBg] = ImVec4(.085, .085, .085, 1);
		colors[ImGuiCol_PopupBg] = ImVec4(.080, .080, .080, 1);
		colors[ImGuiCol_WindowBg] = ImVec4(.065, .065, .065, 1);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(.075, .075, .075, 1);
		colors[ImGuiCol_TitleBg] = ImVec4(.075, .075, .075, 1);
		colors[ImGuiCol_TitleBgActive] = ImVec4(.075, .075, .075, 1);
		colors[ImGuiCol_ChildWindowBg] = ImVec4(.075, .075, .075, 1);
		colors[ImGuiCol_CheckMark] = ImVec4( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 200 / 255.f, 150 / 255.f, 255 / 255.f , 1.f);

		create_objects(device);
	}

	void apply_fonts() {
		ImGui::CreateContext();

		font_title = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\VerdanaB.ttf", 13);
		font_menu = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\VerdanaB.ttf", 12);
		font_big = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\VerdanaB.ttf", 26);
		font_medium = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\VerdanaB.ttf", 14);
		font_small = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\VerdanaB.ttf", 12);
	}

	void __stdcall end_present(IDirect3DDevice9* device) {
		if (opened) {
			if (ImGui::GetStyle().Alpha > 1.f)
				ImGui::GetStyle().Alpha = 1.f;
			else if (ImGui::GetStyle().Alpha != 1.f)
				ImGui::GetStyle().Alpha += 0.05f;
		}

		ImGui::Render();

		state_block->Apply();
		state_block->Release();
	}

	void __stdcall pre_render(IDirect3DDevice9* device) {
		D3DVIEWPORT9 d3d_viewport;
		device->GetViewport(&d3d_viewport);

		device->CreateStateBlock(D3DSBT_ALL, &state_block);
		state_block->Capture();

		device->SetVertexShader(nullptr);
		device->SetPixelShader(nullptr);
		device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		device->SetRenderState(D3DRS_LIGHTING, FALSE);
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

		device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
		device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	}

	void __stdcall post_render() {
		ImGui_ImplDX9_NewFrame();
	}
	ImFont* font_title;
	ImFont* font_menu;
	ImFont* font_big;
	ImFont* font_medium;
	ImFont* font_small;
	bool opened = false;
private:
	ImDrawData _drawData;
	DWORD dwOld_D3DRS_COLORWRITEENABLE;
};

extern c_menu menu;