#include "generated/Variant.h"

#include "VariantInternal.h"
#include "PaintResourcesInternal.h"

#include <QVariant>
#define THIS ((QVariant*)_this)

namespace Variant
{
    bool Handle_isValid(HandleRef _this) {
        return THIS->isValid();
    }

    std::string Handle_toString2(HandleRef _this) {
        return THIS->toString().toStdString();
    }

    int32_t Handle_toInt(HandleRef _this) {
        return THIS->toInt();
    }

    void Handle_dispose(HandleRef _this) {
        printf("Variant::Handle_dispose() - should never be called (in fact should be @nodispose, long story)\n");
    }

    void OwnedHandle_dispose(OwnedHandleRef _this) {
        delete THIS;
    }

    // deferred stuff ========================================
    class FromDeferred : public Deferred::Visitor {
    private:
        QVariant &variant;
    public:
        explicit FromDeferred(QVariant &variant) : variant(variant) {}

        void onEmpty(const Deferred::Empty *empty) override {
            variant = QVariant();
        }

        void onFromBool(const Deferred::FromBool *fromBool) override {
            variant = fromBool->value;
        }

        void onFromString(const Deferred::FromString *fromString) override {
            variant = QString::fromStdString(fromString->value);
        }

        void onFromInt(const Deferred::FromInt *fromInt) override {
            variant = fromInt->value;
        }

        void onFromIcon(const Deferred::FromIcon *fromIcon) override {
            variant = Icon::fromDeferred(fromIcon->value);
        }

        void onFromColor(const Deferred::FromColor *fromColor) override {
            variant = Color::fromDeferred(fromColor->value);
        }
    };

    QVariant fromDeferred(const std::shared_ptr<Deferred::Base>& deferred) {
        QVariant ret;
        FromDeferred visitor(ret);
        deferred->accept(&visitor);
        return ret;
    }
}
