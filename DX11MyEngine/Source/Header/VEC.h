#pragma once
#include <cmath> 
#include <DirectXMath.h>    // 数学系

/*
【外積 VCrossとは】
・垂直な方向を求める
主にローカル軸を求める。前、右、上など
ローカル空間の移動
スプライトのビルボード処理(常にカメラに向ける)

VECTOR forward = { 0,0,1 };
VECTOR up = { 0,1,0 };
VECTOR right = VNorm(VCross(forward, up));

・法線ベクトルを求める(面の向き)
VECTOR a = VSub(p1, p0);
VECTOR b = VSub(p2, p0);
VECTOR normal = VNorm(VCross(a, b));
*/


//
/* 3Dベクトル */
//
namespace VECTOR3
{
	/* 3Dベクトル構造体 */
	struct VEC3
	{
		float x, y, z;

		VEC3() : x(0.f), y(0.f), z(0.f) {};
		VEC3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};
		VEC3(float _scl) :x(_scl), y(_scl), z(_scl) {};
		VEC3(const DirectX::XMFLOAT3& v) :x(v.x), y(v.y), z(v.z) {};    // XMFLOAT3を受け取る際の変換

		// オーバーロード
		VEC3 operator+(const VEC3& vec)const	{ return { this->x + vec.x,this->y + vec.y,this->z + vec.z }; };
		VEC3 operator+(const float &scl)const	{ return { this->x + scl,this->y + scl,this->z + scl }; };
		VEC3 operator-(const VEC3& vec) const	{ return { this->x - vec.x,this->y - vec.y,this->z - vec.z }; };
		VEC3 operator-(const float& scl) const	{ return { this->x - scl,this->y - scl,this->z - scl }; };
		VEC3 operator-() const					{ return { -this->x, -this->y, -this->z }; };
		VEC3 operator*(float scl) const			{ return { this->x * scl,this->y * scl,this->z * scl }; };
		VEC3 operator*(const VEC3& vec) const	{ return { this->x * vec.x,this->y * vec.y,this->z * vec.z }; };
		VEC3 operator+() const					{ return *this; };
		operator DirectX::XMFLOAT3()const		{ return DirectX::XMFLOAT3(x, y, z); }		// XMFLOAT3に渡す際の変換
		operator DirectX::XMVECTOR()const		{ return DirectX::XMVectorSet(x, y, z, 1.0f); }    // XMVECTORに渡す際の変換

		// 複合代入 プラス
		VEC3& operator+=(const VEC3& vec) {
			this->x += vec.x;
			this->y += vec.y;
			this->z += vec.z;

			return *this;
		};
		
		// 複合代入 マイナス
		VEC3& operator-=(const VEC3& vec) {
			this->x -= vec.x;
			this->y -= vec.y;
			this->z -= vec.z;

			return *this;
		};

		// 複合代入 掛け算
		VEC3& operator*=(const VEC3& vec) {
			this->x *= vec.x;
			this->y *= vec.y;
			this->z *= vec.z;

			return *this;
		};
		
		// 複合代入 割り算
		VEC3& operator/=(const VEC3& vec) {
			this->x /= vec.x;
			this->y /= vec.y;
			this->z /= vec.z;

			return *this;
		};


		// 代入演算子 float[4]
		template<size_t N>
		VEC3& operator=(const float(&other)[N]) {
			static_assert(N >= 3, "Array must have at least 3 elements");
			this->x = other[0];
			this->y = other[1];
			this->z = other[2];
			return *this;
		}

		/// <summary>
		/// XMVECTOR から VEC3 への変換
		/// </summary>
		/// <param name="vec"></param>
		/// <returns></returns>
		static VEC3 FromXMVECTOR(const DirectX::XMVECTOR &vec)
		{
			DirectX::XMFLOAT3 tmp;
			DirectX::XMStoreFloat3(&tmp, vec);
			return VEC3(tmp);
		}

		// 全ての要素を1で初期化
		void AllOne()
		{
			this->x = 1.0f;
			this->y = 1.0f;
			this->z = 1.0f;
		};
		// 全ての要素を0で初期化
		void AllZero()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		};

		// 長さ
		float Length()   const { return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z); }
		float LengthSq() const { return this->x * this->x + this->y * this->y + this->z * this->z; };

		// 正規化
		VEC3 Normalize()const {
			float len = Length();
			return (len != 0.0f) ? (*this * (1.0f / len)) : VEC3{ 0,0,0 };
		};

		// 内積
		static float Dot(const VEC3& v1, const VEC3& v2) {
			return  v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		};

		// 外積
		static VEC3 Cross(const VEC3& v1, const VEC3& v2) {
			return {
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x
			};
		};

		// 線形補間
		static VEC3 Lerp(const VEC3& a, const VEC3& b, float t) {
			return a + (b - a) * t;
		};

		// 距離
		static float Distance(const VEC3& v1, const VEC3& v2) {
			return (v2 - v1).Length();
		}
		
		// 距離
		static float DistanceSq(const VEC3& v1, const VEC3& v2) {
			return (v2 - v1).LengthSq();
		}

		// 反射
		static VEC3 Reflect(const VEC3& inVec, const VEC3& normal) {
			return inVec - normal * (2.0f * Dot(inVec, normal));
		}
	};
}


//
/* 4Dベクトル */
//
namespace VECTOR4
{
	/* 4Dベクトル構造体 */
	struct VEC4
	{
		float x, y, z, w;

		VEC4() : x(0.f), y(0.f), z(0.f), w(0.f) {};
		VEC4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {};
		VEC4(float _scl) :x(_scl), y(_scl), z(_scl), w(_scl) {};
		VEC4(const DirectX::XMFLOAT4& v) :x(v.x), y(v.y), z(v.z), w(v.w) {};    // XMFLOAT4を受け取る際の変換
		template<size_t N>
		
		// float[4]からの代入時のコピーコンストラクタ
		VEC4(const float (&other)[N]){
			static_assert(N >= 4, "Array must have at least 4 elements");
			this->x = other[0];
			this->y = other[1];
			this->z = other[2];
			this->w = other[3];
		};

		// オーバーロード
		VEC4 operator+(const VEC4& vec)const { return { this->x + vec.x,this->y + vec.y,this->z + vec.z,this->w + vec.w }; };
		VEC4 operator-(const VEC4& vec) const { return { this->x - vec.x,this->y - vec.y,this->z - vec.z,this->w - vec.w }; };
		VEC4 operator*(float scl) const { return { this->x * scl,this->y * scl,this->z * scl,this->w * scl }; };
		VEC4 operator+() const { return *this; };
		VEC4 operator-() const { return { -this->x, -this->y, -this->z,-this->w }; };
		operator DirectX::XMFLOAT4()const { return DirectX::XMFLOAT4(x, y, z, w); }    // XMFLOAT4に渡す際の変換
		
		// 代入演算子 float[4]
		template<size_t N>
		VEC4& operator=(const float(&other)[N]){
			static_assert(N >= 4, "Array must have at least 4 elements");
			this->x = other[0];
			this->y = other[1];
			this->z = other[2];
			this->w = other[3];
			return *this;
		}

		// 全ての要素を1で初期化
		void AllOne()
		{
			this->x = 1.0f;
			this->y = 1.0f;
			this->z = 1.0f;
			this->w = 1.0f;
		};

		// 全ての要素を0で初期化
		void AllZero()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->w = 0.0f;
		};


		// 長さ
		float Length()   const { return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w); }
		float LengthSq() const { return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w; };

		// 正規化
		VEC4 Normalize()const {
			float len = Length();
			return (len != 0.0f) ? (*this * (1.0f / len)) : VEC4{ 0,0,0,0 };
		};

		// 内積
		static float Dot(const VEC4& v1, const VEC4& v2) {
			return  v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
		};

		// 外積
		static VEC4 Cross(const VEC4& v1, const VEC4& v2) {
			return {
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x,
				0.0f	// w は使わない
			};
		};

		// 線形補間
		static VEC4 Lerp(const VEC4& a, const VEC4& b, float t) {
			return a + (b - a) * t;
		};

		// 距離
		static float Distance(const VEC4& v1, const VEC4& v2) {
			return (v2 - v1).Length();
		}

		// 反射
		static VEC4 Reflect(const VEC4& inVec, const VEC4& normal) {
			return inVec - normal * (2.0f * Dot(inVec, normal));
		}
	};
}


//
/* 2Dベクトル */
//
namespace VECTOR2
{
	/* 2Dベクトル構造体 */
	struct VEC2
	{
		float x, y;
		VEC2() : x(0.f), y(0.f) {};
		VEC2(float _x, float _y) : x(_x), y(_y) {};
		VEC2(DirectX::XMFLOAT2& v) : x(v.x), y(v.y) {}; // XMFLOAT2を受け取る際の変換

		// オーバーロード
		VEC2 operator+(const VEC2& vec)const { return { this->x + vec.x,this->y + vec.y}; };
		VEC2 operator-(const VEC2& vec) const { return { this->x - vec.x,this->y - vec.y }; };
		VEC2 operator*(float scl) const { return { this->x * scl,this->y * scl }; };
		VEC2 operator+() const { return *this; };
		VEC2 operator-() const { return { -this->x, -this->y }; };
		operator DirectX::XMFLOAT2()const { return DirectX::XMFLOAT2(x, y); }    // XMFLOAT2に渡す際の変換


		// 複合代入 プラス
		VEC2& operator+=(const VEC2& vec) {
			this->x += vec.x;
			this->y += vec.y;

			return *this;
		};

		// 複合代入 マイナス
		VEC2& operator-=(const VEC2& vec) {
			this->x -= vec.x;
			this->y -= vec.y;

			return *this;
		};

		// 複合代入 掛け算
		VEC2& operator*=(const VEC2& vec) {
			this->x *= vec.x;
			this->y *= vec.y;

			return *this;
		};

		// 複合代入 割り算
		VEC2& operator/=(const VEC2& vec) {
			this->x /= vec.x;
			this->y /= vec.y;

			return *this;
		};



		// 全ての要素を1で初期化
		void AllOne()
		{
			this->x = 1.0f;
			this->y = 1.0f;
		}

		// 全ての要素を0で初期化
		void AllZero()
		{
			this->x = 0.0f;
			this->y = 0.0f;
		}

		// 長さ
		float Length()   const { return std::sqrt(this->x * this->x + this->y * this->y); }
		float LengthSq() const { return this->x * this->x + this->y * this->y; };

		// 正規化
		VEC2 Normalize()const {
			float len = Length();
			return (len != 0.0f) ? (*this * (1.0f / len)) : VEC2{ 0,0 };
		};
	};
}

