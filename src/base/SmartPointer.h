#ifndef _SMART_POINTER_H_
#define _SMART_POINTER_H_

// spクラス（テンプレートスマートポインタクラス）
//  Created by IKD (2005. 9. 2)

// ・ 登録されたオブジェクトポインタの使用者がいなくなったら自動的にdelete
// ・ 通常のポインタ操作演算子「*」「->」「[ ]」が使える。
// ・ 異なるオブジェクトポインタが登録された状態でアップキャストができる

template <class T>
class sp
{
private:
   UINT *m_pRefCnt;   // 参照カウンタへのポインタ
   T* m_pPtr;             // T型のオブジェクトのポインタ

private:
   /////////////////
   // 参照カウンタ増加
   /////
      void AddRef(){(*m_pRefCnt)++;}

   /////////////////
   // 参照カウンタ減少
   /////
      void Release()
      {
         if(--(*m_pRefCnt) == 0){
             delete[] m_pPtr;
             delete m_pRefCnt;
         }
      }

public:
  ///////////////////////
   // デフォルトコンストラクタ
   /////
      explicit sp(T* src=NULL, int add=0)
      {
         m_pRefCnt = new UINT;
         *m_pRefCnt = add;
         m_pPtr = src;
         AddRef();       // 参照カウンタ増加
      }

  //////////////////////////////////
   // コピーコンストラクタ（同型純粋コピー）
   /////
      sp(const sp<T> &src)
      {
         // 相手のポインタをコピー
         m_pRefCnt = src.m_pRefCnt;
         m_pPtr = src.m_pPtr;

         // 自分自身の参照カウンタを増加
         AddRef();
      }

  //////////////////////////////////////
   // コピーコンストラクタ（暗黙的アップキャスト付き）
   /////
      template<class T2> sp(sp<T2> &src)
      {
         // 相手のポインタをコピー
         m_pRefCnt = src.GetRefPtr();
         m_pPtr = src.GetPtr();

         // 自分自身の参照カウンタを増加
         AddRef();
      }

  ///////////////
   // デストラクタ
   /////
     virtual ~sp()
      {
         Release();
      }


//■■■演算子のオーバーロード■■■■■■■■■■■

   /////////////////////////////////
   // =代入演算子（明示的コピー）
   /////
      sp<T>& operator =(const sp<T> &src)
      {
         // 自分自身への代入は不正で意味が無いので
         // 行わない。
         if(src.m_pPtr == m_pPtr)
            return (*this);

         // 自分は他人になってしまうので
         // 参照カウンタを1つ減少
         Release();

         // 相手のポインタをコピー
         m_pRefCnt = src.m_pRefCnt;
         m_pPtr = src.m_pPtr;

         // 新しい自分自身の参照カウンタを増加
         AddRef();

         return (*this);
      }

   /////////////////////////////////
   // =代入演算子（明示的アップキャスト付き）
   /////
      template<class T2> sp& operator =(sp<T2> &src)
      {
         // 自分自身への代入は不正で意味が無いので
         // 行わない。
         if(src.GetPtr() == m_pPtr)
            return (*this);

         // 自分は他人になってしまうので
         // 参照カウンタを1つ減少
         Release();

         // 相手のポインタをコピー
         m_pRefCnt = src.GetRefPtr();
         m_pPtr = src.GetPtr();

         // 新しい自分自身の参照カウンタを増加
         AddRef();

         return (*this);
      }


   ///////////////
   // *間接演算子
   /////
      T& operator *(){ return *m_pPtr;}

   //////////////////////
   // ->メンバ選択演算子
   /////
      T* operator ->(){ return m_pPtr;}

   //////////////////
   // []配列参照演算子
   /////
      T& operator [](int n){ return m_pPtr[n];}

   /////////////////
   // ==比較演算子
   /////
      BOOL operator ==(int val){
         if(m_pPtr == val)
            return TRUE;
         return FALSE;
      }

   /////////////////
   // !=比較演算子
   /////
      BOOL operator !=(int val){
         if(m_pPtr != val)
            return TRUE;
         return FALSE;
      }


//■■■ メンバ関数 ■■■■■■■■■■■

public:
   //////////////////////////
   // ポインタの明示的な登録
   /////
      void SetPtr(T* src = NULL,  int add=0)
      {
         // 参照カウンタを減らした後に再初期化
            Release();
            m_pRefCnt = new UINT;
            *m_pRefCnt = add;
            m_pPtr = src;
            AddRef();
      }

   /////////////////////
   // ポインタの貸し出し
   /////
      T* GetPtr(){return m_pPtr;}


   ///////////////////////////////
   // 参照カウンタへのポインタを取得
   /////
      UINT* GetRefPtr(){return m_pRefCnt;}

};


#endif