#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-nodiscard"

#include "generated/AbstractListModel.h"
#include "VariantInternal.h"
#include "ModelIndexInternal.h"

#include <QAbstractListModel>

#define THIS ((Subclassed*)_this)

namespace AbstractListModel
{
    class Subclassed : public QAbstractListModel {
    private:
        std::shared_ptr<MethodDelegate> methodDelegate;
        MethodMask methodMask;
    public:
        Subclassed(QObject *parent, const std::shared_ptr<MethodDelegate>& methodDelegate, MethodMask mask)
            : QAbstractListModel(parent)
        {
            this->methodDelegate = methodDelegate;
            methodMask = mask;
        }

        // ==== must-implement abstract methods ===========================
        int rowCount(const QModelIndex &parent) const override {
            return methodDelegate->rowCount((ModelIndex::HandleRef)&parent);
        }

        QVariant data(const QModelIndex &index, int role) const override {
            auto deferred = methodDelegate->data((ModelIndex::HandleRef)&index, (ItemDataRole)role);
            return Variant::fromDeferred(deferred);
        }

        // ==== optional methods ==========================================
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
            if (methodMask & MethodMaskFlags::HeaderData) {
                auto deferred = methodDelegate->headerData(section, (Enums::Orientation)orientation, (ItemDataRole)role);
                return Variant::fromDeferred(deferred);
            } else {
                return QAbstractListModel::headerData(section, orientation, role);
            }
        }

        Qt::ItemFlags flags(const QModelIndex &index) const override {
            auto baseFlags = QAbstractListModel::flags(index);
            if (methodMask & MethodMaskFlags::Flags) {
                auto raw = (int)methodDelegate->getFlags((ModelIndex::HandleRef)&index, baseFlags);
                return (Qt::ItemFlags) raw;
            } else {
                return baseFlags;
            }
        }

        bool setData(const QModelIndex &index, const QVariant &value, int role) override {
            if (methodMask & MethodMaskFlags::SetData) {
                return methodDelegate->setData((ModelIndex::HandleRef)&index, (Variant::HandleRef)&value, (ItemDataRole)role);
            } else {
                return QAbstractListModel::setData(index, value, role);
            }
        }

        int columnCount(const QModelIndex &parent) const override {
            if (methodMask & MethodMaskFlags::ColumnCount) {
                return methodDelegate->columnCount((ModelIndex::HandleRef)&parent);
            } else {
                // QAbstractListModel::columnCount() is private, we're technically not supposed to be doing this in AbstractListModel
                return 1;
            }
        }

        // signal emission wrappers
        void emitDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles) {
            emit dataChanged(topLeft, bottomRight, roles);
        }

        void emitHeaderDataChanged(Qt::Orientation orientation, int first, int last) {
            emit headerDataChanged(orientation, first, last);
        }

        // 'interior' (friend handle) functions
        friend void Interior_beginInsertRows(InteriorRef _this, std::shared_ptr<ModelIndex::Deferred::Base> parent, int32_t first, int32_t last);
        friend void Interior_endInsertRows(InteriorRef _this);
        friend void Interior_beginRemoveRows(InteriorRef _this, std::shared_ptr<ModelIndex::Deferred::Base> parent, int32_t first, int32_t last);
        friend void Interior_endRemoveRows(InteriorRef _this);
        friend void Interior_beginResetModel(InteriorRef _this);
        friend void Interior_endResetModel(InteriorRef _this);
    };

    InteriorRef Handle_getInteriorHandle(HandleRef _this) {
        return (InteriorRef)_this;
    }

    void Handle_dispose(HandleRef _this) {
        printf("!! AbstractListModel::Handle_dispose - honoring for now, but figure out if there needs to be a dedicated handle for subclasses, etc.\n");
        delete THIS;
    }

    void Interior_emitDataChanged(InteriorRef _this, std::shared_ptr<ModelIndex::Deferred::Base> topLeft, std::shared_ptr<ModelIndex::Deferred::Base> bottomRight, std::vector<ItemDataRole> roles) {
        QList<int> qRoles;
        for (auto role : roles) {
            qRoles.push_back((int)role);
        }
        THIS->emitDataChanged(ModelIndex::fromDeferred(topLeft), ModelIndex::fromDeferred(bottomRight), qRoles);
    }

    void Interior_emitHeaderDataChanged(InteriorRef _this, Orientation orientation, int32_t first, int32_t last) {
        THIS->emitHeaderDataChanged((Qt::Orientation)orientation, first, last);
    }

    void Interior_beginInsertRows(InteriorRef _this, std::shared_ptr<ModelIndex::Deferred::Base> parent, int32_t first, int32_t last) {
        THIS->beginInsertRows(ModelIndex::fromDeferred(parent), first, last);
    }

    void Interior_endInsertRows(InteriorRef _this) {
        THIS->endInsertRows();
    }

    void Interior_beginRemoveRows(InteriorRef _this, std::shared_ptr<ModelIndex::Deferred::Base> parent, int32_t first, int32_t last) {
        THIS->beginRemoveRows(ModelIndex::fromDeferred(parent), first, last);
    }

    void Interior_endRemoveRows(InteriorRef _this) {
        THIS->endRemoveRows();
    }

    void Interior_beginResetModel(InteriorRef _this) {
        THIS->beginResetModel();
    }

    void Interior_endResetModel(InteriorRef _this) {
        THIS->endResetModel();
    }

    HandleRef createSubclassed(std::shared_ptr<MethodDelegate> methodDelegate, MethodMask mask) {
        return (HandleRef) new Subclassed(nullptr, methodDelegate, mask);
    }
}

#pragma clang diagnostic pop