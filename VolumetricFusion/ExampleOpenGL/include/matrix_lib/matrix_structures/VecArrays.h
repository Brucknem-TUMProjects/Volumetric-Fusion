#pragma once
#include "matrix_lib/utils/LibIncludeCPU.h"
#include "matrix_lib/utils/VectorMath.h"

namespace matrix_lib {

	/**
	 * 3x3 matrix (row-wise storage).
	 */
	template<bool isInterface>
	class AMat3f: public SoA<
		isInterface,
		typename TL<float4, float4, float4>::type
	> {
	public:
		CPU_AND_GPU float4* xrow() { return t()[I<0>()]; }
		CPU_AND_GPU const float4* xrow() const { return t()[I<0>()]; }
		CPU_AND_GPU float4* yrow() { return t()[I<1>()]; }
		CPU_AND_GPU const float4* yrow() const { return t()[I<1>()]; }
		CPU_AND_GPU float4* zrow() { return t()[I<2>()]; }
		CPU_AND_GPU const float4* zrow() const { return t()[I<2>()]; }
	};

	using iAMat3f = AMat3f<true>;
	using sAMat3f = AMat3f<false>;


	/**
	 * 4x4 matrix (row-wise storage).
	 */
	template<bool isInterface>
	struct AMat4f : public SoA<
		isInterface,
		typename TL<float4, float4, float4, float4>::type
	> {
		CPU_AND_GPU float4* xrow() { return t()[I<0>()]; }
		CPU_AND_GPU const float4* xrow() const { return t()[I<0>()]; }
		CPU_AND_GPU float4* yrow() { return t()[I<1>()]; }
		CPU_AND_GPU const float4* yrow() const { return t()[I<1>()]; }
		CPU_AND_GPU float4* zrow() { return t()[I<2>()]; }
		CPU_AND_GPU const float4* zrow() const { return t()[I<2>()]; }
		CPU_AND_GPU float4* wrow() { return t()[I<3>()]; }
		CPU_AND_GPU const float4* wrow() const { return t()[I<3>()]; }
	};

	using iAMat4f = AMat4f<true>;
	using sAMat4f = AMat4f<false>;


	/**
	 * N-dim array vector.
	 */
	template<bool isInterface, typename ElementType, unsigned N>
	struct AVecX : public SoA<
		isInterface,
		typename AddElements<N, ElementType, NullType>::type
	> {
	private:
		/**
		 * The return type of the elements is the same for all elements. It could be either ElementType*
		 * (if ElementType is a base type) or ElementType (if ElementType is an SoA). This method
		 * returns the suitable return type.
		 */
		template<typename T>
		struct GetReturnType;

		template<typename TList>
		struct GetReturnType<Tuple<TList>> {
			using type = typename TypeAt<0, TList>::type;
		};

	public:
		using BaseSoAType = SoA<isInterface, typename AddElements<N, ElementType, NullType>::type>;
		using ReturnType = typename GetReturnType<typename BaseSoAType::PointerTupleType>::type;

		/* Compile-time indexing operator. */
		template<unsigned Idx> CPU_AND_GPU ReturnType& operator[](I<Idx>) { return t()[I<Idx>()]; }
		template<unsigned Idx> CPU_AND_GPU const ReturnType& operator[](I<Idx>) const { return t()[I<Idx>()]; }

		/* Run-time indexing operator. */
		CPU_AND_GPU ReturnType& operator[](unsigned idx) {
			runtime_assert(idx < N, "Index out of range.");
			ReturnType* elementPtr{ nullptr };
			static_for<N, FindElement>(&elementPtr, this, idx);
			runtime_assert(elementPtr != nullptr, "Pointer not assigned.");
			return *elementPtr;
		}
		CPU_AND_GPU const ReturnType& operator[](unsigned idx) const {
			runtime_assert(idx < N, "Index out of range.");
			ReturnType* elementPtr{ nullptr };
			static_for<N, FindElement>(&elementPtr, this, idx);
			runtime_assert(elementPtr != nullptr, "Pointer not assigned.");
			return *elementPtr;
		}

	private:
		/**
		 * This method returns the suitable element, given the index at run-time.
		 */
		struct FindElement {
			template<int Idx> 
			CPU_AND_GPU static void f(ReturnType** elementPtr, AVecX<isInterface, ElementType, N>* thisPtr, unsigned runtimeIdx) {
				if (runtimeIdx == Idx) *elementPtr = &thisPtr->t()[I<Idx>()];
			}
		};
	};

} // namespace matrix_lib