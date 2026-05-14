#pragma once

// ***************************************************************************************
// ---------------------------------------------------------------------------------------
/* --- @:ConstantBuffer Class --- */
//
//  ★継承：IComponent ★
//
// 【?】定数バッファ
//		
// ***************************************************************************************
template<typename T>
class ConstantBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuff;	// バッファ
	D3D11_USAGE m_Usage;							// 使用方法

public:
	// コンパイル時に16バイトアライメントをチェック
	static_assert((sizeof(T) % 16) == 0, "Constant Buffer struct must be 16-byte aligned.");

	//*---------------------------------------------------------------------------------------
	//*【?】コンストラクタ
	//*----------------------------------------------------------------------------------------
	ConstantBuffer() :
		m_pBuff(nullptr) {
	}

	//*---------------------------------------------------------------------------------------
	//*【?】デストラクタ
	//*----------------------------------------------------------------------------------------
	~ConstantBuffer() {
		if (m_pBuff) {
			m_pBuff->Release();
			m_pBuff = nullptr;
		}
	}
	
	/// <summary>
	/// セットアップ
	/// </summary>
	/// <param name="_device">デバイス</param>
	/// <param name="_usage">使用方法</param>
	/// <returns></returns>
	bool Setup(ID3D11Device* _device, const D3D11_USAGE _usage = D3D11_USAGE_DEFAULT)
	{
		m_Usage = _usage;

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = m_Usage;
		bd.ByteWidth = sizeof(T);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = m_Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;	// 動的変更する場合はCPUアクセスフラグをセット

		HRESULT hr = _device->CreateBuffer(&bd, nullptr, m_pBuff.GetAddressOf());
		if (FAILED(hr)) {
			return false;
		}
		return true;
	}

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_context"></param>
	/// <param name="_data"></param>
	void Update(ID3D11DeviceContext* _context, const T& _data)
	{
		switch (m_Usage)
		{
			/* デフォルト */
		case D3D11_USAGE_DEFAULT:
			_context->UpdateSubresource(m_pBuff.Get(), 0, nullptr, &_data, 0, 0); 
			break;
		
		case D3D11_USAGE_IMMUTABLE:
			break;

			/* 動的書き込み */
		case D3D11_USAGE_DYNAMIC:
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			if (SUCCEEDED(_context->Map(m_pBuff.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			{
				memcpy(mappedResource.pData, &_data, sizeof(T));
				_context->Unmap(m_pBuff.Get(), 0);
			}
		}
			break;
		
		case D3D11_USAGE_STAGING:
			break;
		
		default:
			break;
		}
	}


	ID3D11Buffer* GetBuffer() const { return m_pBuff.Get(); }	// バッファの取得
};

