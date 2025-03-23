#pragma once
#include <Windows.h>
#include <d3d11_3.h>
#include <dxgi1_4.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "../../Module/Modules/ESP.h"
#include "../../Module/Module.hpp"
#include "../../Manager.hpp"
#include "../Hook.h"
#include "../../Utils.hpp"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
bool init2 = false;
Manager* scMgr;

HRESULT __stdcall hkPresent(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
	auto callRendr = [&]() {
		if (scMgr != nullptr) {
			for (auto& mod : scMgr->modules) {
				if (mod->enabled) mod->onImgui();
			}
		}
	};

	if (!init)
	{
		DXGI_SWAP_CHAIN_DESC sd;
		pSwapChain->GetDesc(&sd);
		window = sd.OutputWindow;
		oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
		InitImGui();
		init = true;
		return oPresent(pSwapChain, SyncInterval, Flags);
	}

	static int localPlayerPtr = 0;
	static int worldPtr = 0;

	if (!init2)
	{
		localPlayerPtr = *(int*)(Utils::FindSignature("A1 ?? ?? ?? ?? 8B 40 ?? 85 C0 74 ?? 0F 28 ?? ?? EB 07 0F 28 05 ?? ?? ?? ?? 80") + 1);

		worldPtr = *(int*)(Utils::FindSignature("A1 ?? ?? ?? ?? 85 C0 74 07 C6 80 59 01 00 00 01 5D C2 04 00") + 1);

		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);

		ID3D11Texture2D* pBackBuffer;
		pDevice->GetImmediateContext(&pContext);
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);

		pBackBuffer->Release();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);
		init2 = true;
	}

	// cursed
	auto ESPmod = scMgr->getModule<ESP>();
	if (ESPmod) {
		RECT windowRect;
		GetWindowRect(window, &windowRect);
		int width = windowRect.right - windowRect.left;
		int height = windowRect.bottom - windowRect.top;

		ESPmod->x = width;
		ESPmod->y = height;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	callRendr();

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}

typedef HRESULT(__stdcall* resize_buffers_t)(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
resize_buffers_t original_resize_buffers;

HRESULT __stdcall resize_buffers_callback(IDXGISwapChain3* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags) {
	//reinit ImGui dx11
	//init = false;
	pContext->Release();
	mainRenderTargetView->Release();
	init2 = false;
	return original_resize_buffers(swap_chain, buffer_count, width, height, new_format, swap_chain_flags);
}

class PresentHook : public Hook {
public:
	PresentHook(Manager* mgr) : Hook(mgr) {
		scMgr = mgr;

		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			kiero::bind(13, (void**)&original_resize_buffers, resize_buffers_callback);
		}
	}
};
