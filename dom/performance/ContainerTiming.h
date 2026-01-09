/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ContainerTiming_h___
#define mozilla_dom_ContainerTiming_h___

#include "mozilla/dom/DOMRect.h"
#include "mozilla/dom/PerformanceContainerTimingBinding.h"
#include "mozilla/dom/PerformanceEntry.h"
#include "nsCycleCollectionParticipant.h"

class nsTextFrame;
namespace mozilla::dom {

class Performance;
class PerformanceMainThread;

// https://bloomberg.github.io/container-timing/
class ContainerTiming final : public PerformanceEntry {
 public:
  NS_DECL_ISUPPORTS_INHERITED

  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(ContainerTiming, PerformanceEntry)

  ContainerTiming(Performance* aPerformance, const DOMString& aIdentifier,
                  DOMRectReadOnly* aIntersectionRect, unsigned long aSize,
                  DOMHighResTimeStamp aStartTime,
                  DOMHighResTimeStamp aFirstRenderTime,
                  Element* aLastPaintedElement);

  JSObject* WrapObject(JSContext* aCx,
                       JS::Handle<JSObject*> aGivenProto) override;

  DOMHighResTimeStamp FirstRenderTime() const;
  DOMHighResTimeStamp StartTime() const override;
  DOMRectReadOnly* IntersectionRect() const { return mIntersectionRect.get(); }
  DOMHighResTimeStamp PaintTime() const { return mStartTime; }
  Nullable<DOMHighResTimeStamp> GetPresentationTime() const { return nullptr; }

  unsigned long Size() const { return mSize; }
  void GetIdentifier(nsAString& aIdentifier) const {
    if (mIdentifier) {
      mIdentifier->ToString(aIdentifier);
    }
  }

  Element* GetLastPaintedElement() const;

 private:
  ~ContainerTiming() = default;

  RefPtr<PerformanceMainThread> mPerformance;

  DOMHighResTimeStamp mFirstRenderTime;
  DOMHighResTimeStamp mStartTime;
  unsigned long mSize;
  RefPtr<DOMRectReadOnly> mIntersectionRect;

  RefPtr<Element> mLastPaintedElement;
  RefPtr<nsAtom> mIdentifier;
};
}  // namespace mozilla::dom
#endif
